import robin_stocks.robinhood as r
import json
import os
from datetime import datetime, timedelta
from typing import Optional, List, Dict

from utils.date_utils import generate_expected_intervals
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

    def get_historical_data(self, symbol: str, interval: str = '5minute', span: str = 'week') -> Optional[List[Dict]]:
        Logger.debug_print(f"Getting historical data for the last {span} every {interval} for {symbol}...")
        historicals = r.crypto.get_crypto_historicals(symbol, interval, span)
        if historicals:
            Logger.debug_print(f"Retrieved {len(historicals)} historical records.")
            return historicals
        else:
            Logger.debug_print("No historical data could be retrieved.")
            return None

    def get_historical_data_from_last_7days_5min(self, symbol: str, data_dir: str = "data") -> None:
        data = self.get_historical_data(symbol, interval='5minute', span='week')
        if data:
            self.save_historic_data(symbol, data, interval="5minute", directory=data_dir)

    def save_historic_data(self, symbol: str, object_list, interval="5minute", directory="data"):
        self.save_objects_by_date_and_symbol(symbol, object_list, interval=interval, directory=directory)

    def check_data_completeness(self, symbol: str, date: str, interval: str, directory="data") -> bool:
        return self.is_data_complete_for_day(symbol, date, interval, directory)

    def save_objects_by_date_and_symbol(self, symbol: str, object_list, interval="5minute", directory="data"):
        out_directory = os.path.join(directory, "robinhood", symbol)
        os.makedirs(out_directory, exist_ok=True)

        for obj in object_list:
            try:
                begins_at = obj['begins_at']
                date_str = begins_at.split('T')[0]
                symbol = obj.get('symbol', 'UNKNOWN_SYMBOL')

                filename = f"{symbol}_{date_str}_day_{interval}.json"
                filepath = os.path.join(out_directory, filename)

                try:
                    with open(filepath, 'r') as f:
                        existing_data = json.load(f)
                    existing_begins = {item['begins_at'] for item in existing_data}
                except (FileNotFoundError, json.JSONDecodeError):
                    existing_data = []
                    existing_begins = set()

                if begins_at not in existing_begins:
                    existing_data.append(obj)
                    with open(filepath, 'w') as f:
                        json.dump(existing_data, f, indent=4)

            except KeyError as e:
                print(f"[!] Error: Missing key '{e}' in object.")
            except Exception as e:
                print(f"[!] Error processing object: {e}")
                
    def is_data_complete_for_day(self, symbol: str, date: str, interval: str, directory: str) -> bool:
        filename = f"{symbol}USD_{date}_day_{interval}.json"
        filepath = os.path.join(directory, "robinhood", symbol, filename)

        if not os.path.exists(filepath):
            print(f"[!] File not found: {filepath}")
            return False

        try:
            with open(filepath, 'r') as f:
                data = json.load(f)
        except json.JSONDecodeError:
            print(f"[!] JSON decoding error in: {filepath}")
            return False

        freq_map = {
            "5minute": 300,
            "10minute": 600
        }

        if interval not in freq_map:
            print(f"[!] Unsupported interval: {interval}")
            return False

        interval_seconds = freq_map[interval]

        timestamps = []
        for item in data:
            try:
                dt_str = item['begins_at']
                dt = datetime.strptime(dt_str, "%Y-%m-%dT%H:%M:%SZ")
                timestamps.append(dt)
            except KeyError:
                print("[!] Missing 'begins_at' key in a record.")
                continue
            except Exception as e:
                print(f"[!] Error parsing date: {e}")
                continue

        if not timestamps:
            print("[!] No valid timestamps found in file.")
            return False

        start_date = datetime.strptime(date, "%Y-%m-%d")
        end_date = start_date + timedelta(days=1)

        expected_times = generate_expected_intervals(interval_seconds, start_date, end_date)
        actual_times = set(dt.strftime("%Y-%m-%d %H:%M") for dt in timestamps)

        missing_times = expected_times - actual_times

        if missing_times:
            print(f"[!] Missing data for {len(missing_times)} intervals:")
            for t in sorted(missing_times)[:5]:
                print(f"   - {t}")
            if len(missing_times) > 5:
                print(f"   ... and {len(missing_times) - 5} more")
            return False

        print(f"[✓] All data for {symbol} on {date} is complete.")
        return True