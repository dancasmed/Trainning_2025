from collections import defaultdict
from datetime import timedelta, datetime
import json
import os
import requests


class CoingeckoClient:
    
    symbol_to_id = {
        "btc": "bitcoin",
        "eth": "ethereum",
        "sol": "solana",
        "xrp": "ripple",
        "doge": "dogecoin",
        "ltc": "litecoin",
        "ada": "cardano",
        "dot": "polkadot",
        "shib": "shiba-inu",
    }
    
    def __init__(self):
        pass

    def download_coingecko_marketchart_last_24h_5min(self, symbol: str, data_dir: str = "data/cache") -> None:
        symbol = symbol.lower()

        coin_id = CoingeckoClient.symbol_to_id.get(symbol)
        if not coin_id:
            print(f"[ERROR] Símbolo no reconocido: {symbol}")
            return

        now = datetime.utcnow()
        start_time = now - timedelta(hours=24)
        end_time = now

        url = f"https://api.coingecko.com/api/v3/coins/{coin_id}/market_chart/range"
        params = {
            "vs_currency": "usd",
            "from": int(start_time.timestamp()),
            "to": int(end_time.timestamp())
        }

        try:
            r = requests.get(url, params=params)
            r.raise_for_status()
            data = r.json()
        except requests.exceptions.RequestException as e:
            print(f"[ERROR] Error al obtener datos de CoinGecko: {e}")
            return

        out_dir = os.path.join(data_dir, f"coingecko/{symbol.upper()}")
        os.makedirs(out_dir, exist_ok=True)
        
        # Crear diccionarios para acceso rápido por timestamp
        prices_dict = {ts: price for ts, price in data.get("prices", [])}
        market_caps_dict = {ts: cap for ts, cap in data.get("market_caps", [])}
        total_volumes_dict = {ts: vol for ts, vol in data.get("total_volumes", [])}
        
        # Combinar todos los timestamps únicos
        all_timestamps = set()
        all_timestamps.update(prices_dict.keys())
        all_timestamps.update(market_caps_dict.keys())
        all_timestamps.update(total_volumes_dict.keys())
        sorted_timestamps = sorted(all_timestamps)
        
        # Agrupar datos por día
        grouped = defaultdict(list)
        
        for ts in sorted_timestamps:
            dt = datetime.utcfromtimestamp(ts / 1000).replace(second=0, microsecond=0)
            dt_iso = dt.strftime("%Y-%m-%dT%H:%M:%SZ")
            date_key = dt.strftime("%Y_%m_%d")
            
            entry = {
                "date_time": dt_iso,
                "price": prices_dict.get(ts),
                "market_cap": market_caps_dict.get(ts),
                "total_volume": total_volumes_dict.get(ts)
            }
            grouped[date_key].append(entry)
        
        # Guardar datos combinados por día
        for date_key, new_entries in grouped.items():
            filename = f"{symbol.upper()}USD_marketchart_{date_key}_day_5minute.json"
            filepath = os.path.join(out_dir, filename)
            
            if os.path.exists(filepath):
                with open(filepath, "r") as f:
                    existing_data = json.load(f)
                existing_datetimes = {entry["date_time"] for entry in existing_data}
                
                # Filtrar entradas nuevas
                new_entries = [
                    entry for entry in new_entries
                    if entry["date_time"] not in existing_datetimes
                ]
                
                if not new_entries:
                    print(f"[INFO] No hay datos nuevos para {date_key}")
                    continue
                    
                final_data = existing_data + new_entries
            else:
                final_data = new_entries
            
            # Ordenar por fecha antes de guardar
            final_data.sort(key=lambda x: x["date_time"])
            
            with open(filepath, "w") as f:
                json.dump(final_data, f, indent=2)
            
            print(f"[SUCCESS] Guardado combinado → {filepath}")
    
    def download_coingecko_ohlcv_last_24h_5min(self, symbol: str, data_dir: str = "data/cache") -> None:
        symbol = symbol.lower()
        coin_id = CoingeckoClient.symbol_to_id.get(symbol)
        if not coin_id:
            print(f"[ERROR] Símbolo no reconocido: {symbol}")
            return

        url = f"https://api.coingecko.com/api/v3/coins/{coin_id}/ohlc"
        params = {
            "vs_currency": "usd",
            "days": 1,  # solo se permite 1 día para 5m
            "interval": "5m"
        }

        try:
            print(f"[INFO] Descargando OHLC 5m para {symbol.upper()} desde {url}")
            response = requests.get(url, params=params)
            response.raise_for_status()
            raw_data = response.json()
        except requests.exceptions.RequestException as e:
            print(f"[ERROR] Error al obtener OHLC de CoinGecko: {e}")
            return

        # Preparar directorio
        out_dir = os.path.join(data_dir, f"coingecko/{symbol.upper()}")
        os.makedirs(out_dir, exist_ok=True)

        formatted = []
        seen = set()

        for item in raw_data:
            timestamp = item[0] / 1000
            dt = datetime.utcfromtimestamp(timestamp).replace(second=0, microsecond=0)
            dt_iso = dt.strftime("%Y-%m-%dT%H:%M:%SZ")
            date_key = dt.strftime("%Y_%m_%d")

            if dt_iso in seen:
                continue
            seen.add(dt_iso)

            formatted.append({
                "date_time": dt_iso,
                "open": item[1],
                "high": item[2],
                "low": item[3],
                "close": item[4],
                "symbol": symbol.upper()
            })

        if not formatted:
            print(f"[WARN] No se obtuvieron datos para {symbol.upper()}")
            return

        # Agrupar por día
        grouped = defaultdict(list)
        for entry in formatted:
            date_key = entry["date_time"][:10].replace("-", "_")
            grouped[date_key].append(entry)

        for date_key, entries in grouped.items():
            filename = f"{symbol.upper()}USD_ohlc_{date_key}_5minute.json"
            filepath = os.path.join(out_dir, filename)

            if os.path.exists(filepath):
                with open(filepath, "r") as f:
                    existing_data = json.load(f)
                existing_datetimes = {e["date_time"] for e in existing_data}
                new_entries = [e for e in entries if e["date_time"] not in existing_datetimes]
                final_data = existing_data + new_entries
            else:
                final_data = entries

            final_data.sort(key=lambda x: x["date_time"])

            with open(filepath, "w") as f:
                json.dump(final_data, f, indent=2)

            print(f"[SUCCESS] OHLC 5m guardado → {filepath}")