
# api/robinhood_client.py
import robin_stocks.robinhood as r

import json
import os
from datetime import datetime, timedelta

def debug_print(msg):
    print(f"[DEBUG] {msg}")

class RobinhoodClient:
    def __init__(self, username: str, password: str):
        debug_print("Logging into Robinhood...")
        loginResponse = r.login(username, password)
        if 'access_token' not in loginResponse:
            debug_print("Error while logging in, verify user/password or 2FA.")
            exit(1)
        debug_print("Session logged in successfully.")
        
    def __del__(self):
        debug_print("Logging out of Robinhood session...")
        r.logout()
        debug_print("Session logged out successfully.")

    def get_current_values(self, symbol: str):
        debug_print(f"Getting current quote for {symbol}...")
        quote = r.crypto.get_crypto_quote(symbol)
        debug_print(quote)
        if quote:
            print(f"Current price of {symbol}: {quote.get('mark_price')}")
        else:
            debug_print("Could not retrieve current values.")
            
    def get_historical_data(self, symbol: str, interval: str = '10minute', span: str = 'week'):
        debug_print(f"Getting historical data for the last {span} every {interval} for {symbol}...")
        historicals = r.crypto.get_crypto_historicals(symbol, interval, span)
        if historicals:
            debug_print(f"Retrieved {len(historicals)} historical records.")
            self.save_historic_data(historicals, interval=interval, directory="data/cache")
            return historicals
        else:
            debug_print("No historical data could be retrieved.")
            return None

    def get_historical_data_last_week_10_min(self, symbol: str):
        return self.get_historical_data(symbol, interval='10minute', span='week')

    def get_historical_data_last_week_5min(self, symbol: str):
        return self.get_historical_data(symbol, interval='5minute', span='week')

    def save_historic_data(self, object_list, interval="5minute", directory="."):
        os.makedirs(directory, exist_ok=True)

        for obj in object_list:
            try:
                begins_at = obj['begins_at']
                date_str = begins_at.split('T')[0]

                # Get symbol from the object, with fallback
                symbol = obj.get('symbol', 'UNKNOWN_SYMBOL')

                # Construct the filename
                filename = f"{symbol}_{date_str}_day_{interval}.json"
                filepath = os.path.join(directory, filename)

                # Load existing data from file, if any
                try:
                    with open(filepath, 'r') as f:
                        existing_data = json.load(f)
                    existing_begins = {item['begins_at'] for item in existing_data}
                except (FileNotFoundError, json.JSONDecodeError):
                    existing_data = []
                    existing_begins = set()

                # Add new object if not already present
                if begins_at not in existing_begins:
                    existing_data.append(obj)

                    # Write updated data to file
                    with open(filepath, 'w') as f:
                        json.dump(existing_data, f, indent=4)
                    #print(f"[✓] Object added to {filename}")
                # else:
                #     print(f"[-] Object with begins_at='{begins_at}' already exists in {filename}")

            except KeyError as e:
                print(f"[!] Error: Missing key '{e}' in object.")
            except Exception as e:
                print(f"[!] Error processing object: {e}")

    def check_data_completeness(self, symbol: str, date: str, interval: str, directory="."):
        """
        Checks if the data for the given day and symbol is complete according to the frequency.

        Args:
            symbol (str): Asset symbol (e.g., 'BTCUSD').
            date (str): Date to check in YYYY-MM-DD format.
            interval (str): Data frequency ("10_min" or "5_min").
            directory (str): Directory where JSON files are stored.

        Returns:
            bool: True if data is complete, False otherwise.
        """
        filename = f"{symbol}USD_{date}_day_{interval}.json"
        filepath = os.path.join(directory, filename)

        if not os.path.exists(filepath):
            print(f"[!] File not found: {filepath}")
            return False

        try:
            with open(filepath, 'r') as f:
                data = json.load(f)
        except json.JSONDecodeError:
            print(f"[!] JSON decoding error in: {filepath}")
            return False

        # Map frequency to seconds
        freq_seconds = {
            "10minute": 600,
            "5minute": 300
        }

        if interval not in freq_seconds:
            print(f"[!] Unsupported interval: {interval}")
            return False

        interval_seconds = freq_seconds[interval]

        # Parse begins_at timestamps to datetime objects
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

        # Define start and end of the day
        start_date = datetime.strptime(date, "%Y-%m-%d")
        end_date = start_date + timedelta(days=1)

        # Generate all expected intervals
        expected_times = set()
        current = start_date
        while current < end_date:
            expected_times.add(current.strftime("%Y-%m-%d %H:%M"))
            current += timedelta(seconds=interval_seconds)

        # Extract actual timestamps used in data
        actual_times = set(dt.strftime("%Y-%m-%d %H:%M") for dt in timestamps)

        # Find missing timestamps
        missing_times = expected_times - actual_times

        if missing_times:
            print(f"[!] Missing data for {len(missing_times)} intervals.")
            return False

        print(f"[✓] All data for {symbol} on {date} is complete.")
        return True