import os
import json
from typing import List, Dict
from datetime import datetime, timedelta
from utils.date_utils import generate_expected_intervals

def save_objects_by_date_and_symbol(object_list, interval="5minute", directory="."):
    os.makedirs(directory, exist_ok=True)

    for obj in object_list:
        try:
            begins_at = obj['begins_at']
            date_str = begins_at.split('T')[0]
            symbol = obj.get('symbol', 'UNKNOWN_SYMBOL')

            filename = f"{symbol}_{date_str}_day_{interval}.json"
            filepath = os.path.join(directory, filename)

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
            
def is_data_complete_for_day(symbol: str, date: str, interval: str, directory: str) -> bool:
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

def save_news_by_date_and_asset(news_list, asset, directory="."):
    """
    Saves each news item into a JSON file named by asset and published date.
    Avoids duplicates based on 'published_date' and 'title'.

    Filename format: {asset}_{YYYY-MM-DD}_news.json
    """
    os.makedirs(directory, exist_ok=True)

    for item in news_list:
        try:
            pub_date = item['published_date']
            date_str = pub_date.split('T')[0] if 'T' in pub_date else pub_date

            filename = f"{asset}_{date_str}_news.json"
            filepath = os.path.join(directory, filename)

            try:
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
            # else:
            #     print(f"[-] Duplicate news item skipped: {item['title']}")

        except KeyError as e:
            print(f"[!] Missing key '{e}' in news item.")
        except Exception as e:
            print(f"[!] Error processing news item: {e}")