from typing import Dict, List
from api.robinhood_client import RobinhoodClient

from news import news_fetcher
from news.news_pipeline import NewsPipeline

import os

from utils.html_to_markdown import url_to_markdown



DATA_DIR = "data/cache"
os.makedirs(DATA_DIR, exist_ok=True)

def main():
    username = "mail"
    password = "password"

    crypto_symbols = ["DOGE", "BTC", "ETH", "XRP", "LTC", "SOL", "ADA", "DOT", "SHIB"]
    
    rh_client = RobinhoodClient(username, password)    
    pipeline = NewsPipeline(ollama_model="gemma3:27b")  # Puedes cambiar modelo si deseas
    
    for symbol in crypto_symbols:
        interval = "5minute"
        
        # Get current price
        rh_client.get_current_values(symbol)
        
        # Download historical data
        rh_client.get_historical_data_from_last_7days_5min(symbol)

        # Check completeness
        rh_client.check_data_completeness(symbol, "2025-06-02", interval, DATA_DIR)
        rh_client.check_data_completeness(symbol, "2025-06-03", interval, DATA_DIR)
        
        # Get sentiment from LLM + News
        
        
        print(f"\n🔎 Getting news related to {symbol} ...")
        news_items = pipeline.get_news_from_last_7days(symbol)
        
        print(f"\n📊 Analyzing sentiment for {symbol} ...")
        sentiment, news_items = pipeline.get_sentiment_for_asset(symbol, DATA_DIR)
        print(f"\n📈 Sentiment for {symbol}: {sentiment.upper()}")

if __name__ == "__main__":
    main()