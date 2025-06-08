from concurrent.futures import ThreadPoolExecutor, as_completed
import html
import json
import os
import threading
import time

from llm.ollama_client import OllamaClient
from utils.html_to_markdown import url_to_markdown
from utils.logger import Logger
from datetime import datetime, timedelta
from bs4 import BeautifulSoup

from selenium import webdriver
from selenium.webdriver.chrome.options import Options
from selenium.webdriver.chrome.service import Service
from utils.logger import Logger
from webdriver_manager.chrome import ChromeDriverManager

import requests

class GoogleNewsRssClient:
    MAX_THREADS = 5
    
    _file_lock = threading.Lock()

    def __init__(self, ollama_model: str = "gemma3:27b"):
        self.llm_client = OllamaClient(model=ollama_model)

    def get_news_from_last_7days(self, symbol: str, data_dir="data/cache"):
        Logger.debug_print(f"🧠 Generando consulta de búsqueda para el activo: {symbol}")
        query = self.llm_client.generate_search_query(symbol)

        print(f"\n🧠 Prompt generado para búsqueda de noticias: '{query}'")

        # Fetch news
        Logger.debug_print(f"🔍 Buscando noticias ...")
        news_items = self.fetch_raw_news(query)
        
        Logger.debug_print("Downloading news items...")
        parsed_news = self.parse_news_items(symbol, news_items, data_dir)
        
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
    
    def parse_news_items(self, symbol: str, filtered_news, data_dir="data/cache"):
        # Usamos ThreadPoolExecutor con máximo de hilos
        with ThreadPoolExecutor(max_workers=GoogleNewsRssClient.MAX_THREADS) as executor:
            futures = {executor.submit(self.parse_news_item, symbol, item, data_dir): item for item in filtered_news}

        for future in as_completed(futures):
            try:
                future.result()
                pass
            except Exception as exc:
                print(f"{futures[future]} generó una excepción: {exc}")
                
        return filtered_news

    def parse_news_item(self, symbol: str, item, data_dir="data/cache"):
        try:
            Logger.debug_print(f"Parsing news item for symbol: {symbol}...")
            
            title = item.title.text.strip() if item.title else "No title"
            link = item.link.text.strip() if item.link else "#" 
            pub_date_raw = item.pubDate.text.strip() if item.pubDate else None
            # Parsear fecha y hora
            published_date, published_time = self.parse_pub_date(pub_date_raw)
            item.published_date = published_date
            item.title = title
            
            if self._item_exists_in_file(item, symbol, data_dir):
                Logger.debug_print(f"[!] News item already exists for {symbol} in cache. Skipping...")
                return
            
            Logger.debug_print(f"New item found for {symbol}. Parsing details...")

            source = item.source.text.strip() if item.source else "Unknown source"

            summary = ""
            if item.description:
                raw_summary = item.description.text.strip()
                summary = html.unescape(raw_summary)
                summary = BeautifulSoup(summary, "html.parser").get_text()
                summary = summary.split("...")[0] + "..." if "..." in summary else summary

            Logger.debug_print(f"Getting real url for: {title})")
            final_url = self.get_final_url_from_google_news(link)
            Logger.debug_print(f"Final URL resolved: {final_url}")

            Logger.debug_print(f"Generating markdown version for: {title} ({final_url})...")
            markdown = url_to_markdown(final_url)
            if markdown.startswith("Error al procesar la URL"):
                Logger.error_print(f"[!] Error processing URL: {final_url}. Skipping item.")
                return
            
            Logger.debug_print(f"Classifying sentiment for: {title}...")
            sentiment = self.llm_client.classify_sentiment(markdown)

            new_news_item = {
                "symbol": symbol,
                "title": title,
                "link": final_url,
                "source": source,
                "summary": summary,
                "published_date": published_date,
                "published_time": published_time,
                "full_text": markdown,
                "sentiment": sentiment  # Update sentiment based on LLM classification
            }
            self.save_news_item(new_news_item, symbol, data_dir)
        except Exception as e:
            Logger.debug_print(f"[!] Error parsing news item: {e}")
            
    
    def _item_exists_in_file(self, item, symbol: str, data_dir: str = "data/cache") -> bool:
        try:
            Logger.debug_print(f"Checking if news item exists for symbol: {symbol}...")
            out_dir = os.path.join(data_dir, f"google_news_rss/{symbol.upper()}")

            title = item.title

            filename = f"{symbol}_{item.published_date}_news.json"
            filepath = os.path.join(out_dir, filename)
            
            with self._file_lock:
                with open(filepath, 'r') as f:
                    existing_data = json.load(f)
                existing_keys = {
                    (entry['published_date'], entry['title']) for entry in existing_data
                }
            return (item.published_date, title) in existing_keys
        except (FileNotFoundError, json.JSONDecodeError):
            return False

    def save_news_item(self, item, symbol: str, data_dir: str = "data/cache") -> None:
        Logger.debug_print(f"Saving news item for symbol: {symbol}...")
        
        out_dir = os.path.join(data_dir, f"google_news_rss/{symbol.upper()}")
        os.makedirs(out_dir, exist_ok=True)
        
        pub_date = item['published_date']
        date_str = pub_date.split('T')[0] if 'T' in pub_date else pub_date

        filename = f"{symbol}_{date_str}_news.json"
        filepath = os.path.join(out_dir, filename)
        
        try:
            with self._file_lock:
                existing_data = []
                try:
                    with open(filepath, 'r') as f:
                        existing_data = json.load(f)
                except (FileNotFoundError, json.JSONDecodeError):
                    pass
                    
                existing_data.append(item)
                with open(filepath, 'w') as f:
                    json.dump(existing_data, f, indent=4)
            Logger.debug_print(f"[✓] News item saved to {filename}")
        except Exception as e:
            Logger.error_print(f"[✗] Error saving news item: {e}")

    @staticmethod
    def parse_pub_date(pub_date_str: str) -> (str, str):
        try:
            dt = datetime.strptime(pub_date_str, "%a, %d %b %Y %H:%M:%S %Z")
            return dt.strftime("%Y-%m-%d"), dt.strftime("%H:%M")
        except Exception as e:
            print(f"[!] Error parsing date: {e}")
            return None, None
        
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