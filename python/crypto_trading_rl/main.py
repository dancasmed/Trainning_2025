from typing import Dict, List

from api.BinanceClient import BinanceOHLCVClient
from api.coingecko_client import CoingeckoClient
from api.google_news_rss_client import GoogleNewsRssClient
from api.robinhood_client import RobinhoodClient

import os

from utils.html_to_markdown import url_to_markdown
from utils.logger import Logger
from utils.thermal_helper import get_thermal_pressure_level

import requests
import json
import os
from collections import defaultdict
from datetime import datetime, timedelta
import pandas as pd


DATA_DIR = "data/cache"
os.makedirs(DATA_DIR, exist_ok=True)

def main():
    username = "dancasmed@gmail.com"
    password = "Tenochtitlan2025$"
        
    #crypto_symbols = ["DOGE", "BTC", "ETH", "XRP", "LTC", "SOL", "ADA", "DOT", "SHIB"]
    crypto_symbols = ["BTC"]
    
    rh_client = RobinhoodClient(username, password)
    coingeko_client = CoingeckoClient()
    google_news_rss_client = GoogleNewsRssClient(ollama_model="gemma3:27b")
    
    for symbol in crypto_symbols:
        get_thermal_pressure_level()
        interval = "5minute"

        coingeko_client.download_coingecko_ohlcv_last_24h_5min(symbol, DATA_DIR)
        coingeko_client.download_coingecko_marketchart_last_24h_5min(symbol, DATA_DIR)
        
        # Download historical data
        rh_client.get_historical_data_from_last_7days_5min(symbol, DATA_DIR)
        
        print(f"\n🔎 Getting news related to {symbol} ...")
        google_news_rss_client.get_news_from_last_7days(symbol)
        
        # Check completeness
        rh_client.check_data_completeness(symbol, "2025-06-02", interval, DATA_DIR)
        rh_client.check_data_completeness(symbol, "2025-06-03", interval, DATA_DIR)
        
if __name__ == "__main__":
    main()