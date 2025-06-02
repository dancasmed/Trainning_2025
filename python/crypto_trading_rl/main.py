# main.py
from api.robinhood_client import RobinhoodClient
import robin_stocks.robinhood as r

import json
import os
from datetime import datetime, timedelta

def debug_print(msg):
    print(f"[DEBUG] {msg}")

def main():
    username = "dancasmed@gmail.com"
    password = "Mictlantecutli120$"
    
    crypto_symbols = ["DOGE", "BTC", "ETH", "XRP", "LTC", "SOL", "ADA", "DOT", "SHIB"]
    # Initialize Robinhood client
    rh_client = RobinhoodClient(username, password)

    for symbol in crypto_symbols:
        interval = "5minute"  # Intervalo de tiempo para los datos históricos
        rh_client.get_current_values(symbol)
        
        # Parámetros para históricos:
        # interval puede ser '5minute', '10minute', 'hour', 'day', 'week', 'month'
        # span puede ser 'day', 'week', 'month', '3month', 'year', '5year', 'all'
        historicals = rh_client.get_historical_data_last_week_5min(symbol)

        rh_client.check_data_completeness(symbol, "2025-05-31", interval, "data/cache")
        rh_client.check_data_completeness(symbol, "2025-06-02", interval, "data/cache")

if __name__ == "__main__":
    main()
