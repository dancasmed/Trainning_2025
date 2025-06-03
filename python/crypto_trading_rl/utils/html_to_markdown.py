import requests
from readability import Document
import html2text
from bs4 import BeautifulSoup

# Opcional: usar Selenium si requests falla
from selenium import webdriver
from selenium.webdriver.chrome.options import Options
from webdriver_manager.chrome import ChromeDriverManager
import time

def fetch_html_with_headers(url):
    headers = {
        "User-Agent": (
            "Mozilla/5.0 (Windows NT 10.0; Win64; x64) "
            "AppleWebKit/537.36 (KHTML, like Gecko) "
            "Chrome/123.0.0.0 Safari/537.36"
        ),
        "Accept-Language": "en-US,en;q=0.9",
    }
    response = requests.get(url, headers=headers, timeout=15)
    response.raise_for_status()
    return response.text

def fetch_html_with_selenium(url):
    options = Options()
    options.add_argument("--headless=new")
    options.add_argument("--disable-gpu")
    options.add_argument("--no-sandbox")
    options.add_argument("--log-level=3")

    driver = webdriver.Chrome(ChromeDriverManager().install(), options=options)
    driver.get(url)
    time.sleep(4)
    html = driver.page_source
    driver.quit()
    return html

def url_to_markdown(url: str) -> str:
    try:
        try:
            html = fetch_html_with_headers(url)
        except Exception:
            html = fetch_html_with_selenium(url)

        if not html or len(html.strip()) < 50:
            raise ValueError("Document is empty")

        doc = Document(html)
        title = doc.short_title()
        content_html = doc.summary()

        soup = BeautifulSoup(content_html, "html.parser")
        if not soup.get_text(strip=True):
            raise ValueError("No visible content extracted")

        markdown_converter = html2text.HTML2Text()
        markdown_converter.ignore_links = False
        markdown_converter.bypass_tables = False
        markdown = markdown_converter.handle(content_html)

        return f"# {title}\n\n{markdown}"

    except Exception as e:
        return f"Error al procesar la URL: {e}"
