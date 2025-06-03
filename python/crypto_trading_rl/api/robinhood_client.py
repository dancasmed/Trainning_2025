import robin_stocks.robinhood as r
import json
import os
from datetime import datetime, timedelta
from typing import Optional, List, Dict

from utils.logger import Logger


class RobinhoodClient:
    def __init__(self, username: str, password: str):
        Logger.debug_print("Logging into Robinhood...")
        login_response = r.login(username, password)
        if 'access_token' not in login_response:
            Logger.debug_print("Error while logging in, verify user/password or 2FA.")
            raise RuntimeError("Login failed")
        Logger.debug_print("Session logged in successfully.")

    def __del__(self):
        try:
            Logger.debug_print("Logging out of Robinhood session...")
            r.logout()
            Logger.debug_print("Session logged out successfully.")
        except Exception as e:
            Logger.debug_print(f"Error during logout: {e}")

    def get_current_values(self, symbol: str) -> None:
        Logger.debug_print(f"Getting current quote for {symbol}...")
        quote = r.crypto.get_crypto_quote(symbol)
        if quote:
            print(f"Current price of {symbol}: {quote.get('mark_price')}")
        else:
            Logger.debug_print("Could not retrieve current values.")

    def get_historical_data(
        self,
        symbol: str,
        interval: str = '5minute',
        span: str = 'week'
    ) -> Optional[List[Dict]]:
        Logger.debug_print(f"Getting historical data for the last {span} every {interval} for {symbol}...")
        historicals = r.crypto.get_crypto_historicals(symbol, interval, span)
        if historicals:
            Logger.debug_print(f"Retrieved {len(historicals)} historical records.")
            return historicals
        else:
            Logger.debug_print("No historical data could be retrieved.")
            return None

    def get_historical_data_from_last_7days_5min(self, symbol: str) -> None:
        data = self.get_historical_data(symbol, interval='5minute', span='week')
        if data:
            self.save_historic_data(data, interval="5minute")

    def save_historic_data(self, object_list, interval="5minute", directory="data/cache"):
        from utils.file_utils import save_objects_by_date_and_symbol
        save_objects_by_date_and_symbol(object_list, interval=interval, directory=directory)

    def check_data_completeness(self, symbol: str, date: str, interval: str, directory="data/cache") -> bool:
        from utils.file_utils import is_data_complete_for_day
        return is_data_complete_for_day(symbol, date, interval, directory)