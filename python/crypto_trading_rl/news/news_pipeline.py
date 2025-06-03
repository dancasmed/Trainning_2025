# news/news_pipeline.py
from llm.ollama_client import OllamaClient
from news.news_fetcher import NewsFetcher
from news.sentiment_analyzer import SentimentAnalyzer
from utils.file_utils import save_news_by_date_and_asset
from datetime import datetime, timedelta

from utils.logger import Logger
import os
import json

class NewsPipeline:
    def __init__(self, ollama_model):
        self.llm = OllamaClient(model=ollama_model)
        self.fetcher = NewsFetcher()
        self.analyzer = SentimentAnalyzer(model=ollama_model)

    def get_sentiment_for_asset(self, symbol, directory="data/cache"):
        news_items = []

        # Buscar archivos que comiencen con el símbolo dado
        for filename in os.listdir(directory):
            if filename.startswith(symbol) and filename.endswith("_news.json"):
                file_path = os.path.join(directory, filename)
                try:
                    with open(file_path, "r", encoding="utf-8") as f:
                        data = json.load(f)
                        if isinstance(data, list):
                            for item in data:
                                if not item.get("processed", False):
                                    news_items.append(item)
                        else:
                            Logger.debug_print(f"[!] Skipping non-list file: {filename}")
                except Exception as e:
                    Logger.debug_print(f"[!] Error reading {filename}: {e}")

        Logger.debug_print(f"[✓] Loaded {len(news_items)} news items for [{symbol}].")

        # Análisis de sentimiento
        sentiment = self.analyzer.aggregate_sentiment(news_items, symbol, directory)
        print(f"\n📊 Sentimiento general: {sentiment.upper()}")

        return sentiment, news_items


    def get_news_from_last_7days(self, symbol):
        # Compute date range: last 7 days
        today = datetime.utcnow()
        seven_days_ago = today - timedelta(days=7)

        # Generate search query
        Logger.debug_print(f"🧠 Generando consulta de búsqueda para el activo: {symbol}")
        query = self.llm.generate_search_query(symbol)
        print(f"\n🧠 Prompt generado para búsqueda de noticias: '{query}'")

        # Fetch news
        Logger.debug_print(f"🔍 Buscando noticias ...")
        news_items = self.fetcher.fetch_raw_news(query)
        news_items = self.fetcher.parse_news_items(symbol, news_items)

        Logger.debug_print(f"[✓] Found {len(news_items)} news items for last 7 days.")
        return news_items
