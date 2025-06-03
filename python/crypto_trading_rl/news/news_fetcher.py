# news/news_fetcher.py
import json
import os
from newspaper import Article
import traceback
import html
import requests
import time
import threading

from bs4 import BeautifulSoup
from utils.html_to_markdown import url_to_markdown


from selenium import webdriver
from selenium.webdriver.chrome.options import Options
from selenium.webdriver.chrome.service import Service
from utils.logger import Logger
from webdriver_manager.chrome import ChromeDriverManager

from datetime import datetime, timedelta
from concurrent.futures import ThreadPoolExecutor, as_completed

MAX_THREADS = 10

class NewsFetcher:
    _file_lock = threading.Lock()
    
    def get_final_url_from_google_news(self, google_news_url):
        chrome_options = Options()
        chrome_options.add_argument("--headless")
        chrome_options.add_argument("--disable-gpu")

        service = Service(ChromeDriverManager().install())
        driver = webdriver.Chrome(service=service, options=chrome_options)
        try:
            driver.get(google_news_url)
            time.sleep(3)  # Espera a que se redireccione completamente
            return driver.current_url
        finally:
            driver.quit()

    def download_full_article(self, url: str) -> str:
        """
        Extrae el artículo completo desde el enlace final resolviendo redirección de Google News.
        """
        try:
            print(f"[DEBUG] Resolving final URL from: {url}")
            final_url = url
            print(f"[DEBUG] Final resolved URL: {final_url}")

            article = Article(final_url)
            article.download()
            article.parse()
            full_text = article.text.strip()
            Logger.debug_print("FULLTEXT:")
            Logger.debug_print(full_text)
            return full_text
        except Exception as e:
            print(f"[ERROR] newspaper3k failed: {e}")
            traceback.print_exc()
            return ""

    
    def fetch_raw_news(self, search_query):
        headers = {
            "User-Agent": "Mozilla/5.0"
        }
        
        # Compute date range: last 7 days
        today = datetime.utcnow()
        seven_days_ago = today - timedelta(days=7)

        query = search_query.replace(" ", "+")
        url = f"https://news.google.com/rss/search?q={query}&hl=en-US&gl=US&ceid=US:en"

        try:
            response = requests.get(url, headers=headers)
            response.raise_for_status()
        except requests.RequestException as e:
            Logger.debug_print(f"[!] Error fetching news feed: {e}")
            return []

        soup = BeautifulSoup(response.content, features="xml")
        items = soup.findAll("item")

        if not items:
            Logger.debug_print("[*] No news items found in the feed.")
            return []

        Logger.debug_print(f"[✓] Found {len(items)} news items from multiple dates.")
        
        # Filter news within last 7 days
        filtered_news = []
        for item in items:
            try:
                pub_date_raw = item.pubDate.text.strip() if item.pubDate else None
                # Parsear fecha y hora
                published_date_raw, published_time_raw = self.parse_pub_date(pub_date_raw)

                published_date = datetime.fromisoformat(published_date_raw.replace('Z', '+00:00'))
                if seven_days_ago <= published_date <= today:
                    filtered_news.append(item)
            except Exception as e:
                print(f"[!] Error parsing date for item: {item.get('title', 'N/A')}\n    -> {e}")

        Logger.debug_print(f"[✓] Found {len(filtered_news)} news items for last 7 days.")
        return filtered_news

    def parse_news_items(self, asset, filtered_news):
        news_list = []
            
        # Usamos ThreadPoolExecutor con máximo de hilos
        with ThreadPoolExecutor(max_workers=MAX_THREADS) as executor:
            futures = {executor.submit(self.parse_news_item, asset, item): item for item in filtered_news}
        
        for future in as_completed(futures):
            try:
                resultado = future.result()
                print(f"Resultado final: {resultado}")
            except Exception as exc:
                print(f"{futures[future]} generó una excepción: {exc}")
                
        return news_list
    
    def parse_news_item(self, asset, item):
        try:
            title = item.title.text.strip() if item.title else "No title"
            link = item.link.text.strip() if item.link else "#" 
            pub_date_raw = item.pubDate.text.strip() if item.pubDate else None

            source = item.source.text.strip() if item.source else "Unknown source"

            summary = ""
            if item.description:
                raw_summary = item.description.text.strip()
                summary = html.unescape(raw_summary)
                summary = BeautifulSoup(summary, "html.parser").get_text()
                summary = summary.split("...")[0] + "..." if "..." in summary else summary

            # Parsear fecha y hora
            published_date, published_time = self.parse_pub_date(pub_date_raw)

            Logger.debug_print(f"Getting real url for: {title})")
            final_url = self.get_final_url_from_google_news(link)
            Logger.debug_print(f"Final URL resolved: {final_url}")

            Logger.debug_print(f"Generating markdown version for: {title} ({final_url})...")
            markdown = url_to_markdown(final_url)
            if markdown.startswith("Error al procesar la URL"):
                return

            new_news_item = {
                "asset": asset,
                "title": title,
                "link": final_url,
                "source": source,
                "summary": summary,
                "published_date": published_date,
                "published_time": published_time,
                "full_text": markdown,
                "processed": False,
                "sentiment": "neutral"  # Default sentiment, will be updated later
            }
            self.save_news_item(new_news_item, asset)
        except Exception as e:
            Logger.debug_print(f"[!] Error parsing news item: {e}")
            
    
    def save_news_item(self, item, asset, directory="data/cache"):
        """
        Guarda un artículo de noticias en un archivo JSON.
        Evita duplicados basados en 'published_date' y 'title'.
        """
        os.makedirs(directory, exist_ok=True)
        
        pub_date = item['published_date']
        date_str = pub_date.split('T')[0] if 'T' in pub_date else pub_date

        filename = f"{asset}_{date_str}_news.json"
        filepath = os.path.join(directory, filename)

        try:
            with self._file_lock:
                with open(filepath, 'r') as f:
                    existing_data = json.load(f)
                existing_keys = {
                    (entry['published_date'], entry['title']) for entry in existing_data
                }
        except (FileNotFoundError, json.JSONDecodeError):
            existing_data = []
            existing_keys = set()

        key = (item['published_date'], item['title'])
        if key not in existing_keys:
            existing_data.append(item)
            with open(filepath, 'w') as f:
                json.dump(existing_data, f, indent=4)
            print(f"[✓] News item saved to {filename}")
    
    @staticmethod
    def parse_pub_date(pub_date_str: str) -> (str, str):
        """
        Convierte una fecha RSS (ej. 'Fri, 09 May 2025 07:00:00 GMT')
        en dos campos: fecha (YYYY-MM-DD) y hora (HH:MM).
        """
        try:
            dt = datetime.strptime(pub_date_str, "%a, %d %b %Y %H:%M:%S %Z")
            return dt.strftime("%Y-%m-%d"), dt.strftime("%H:%M")
        except Exception as e:
            print(f"[!] Error parsing date: {e}")
            return None, None
