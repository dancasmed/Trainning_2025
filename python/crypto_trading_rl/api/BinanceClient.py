import requests
import datetime
from typing import List, Dict, Optional

class BinanceOHLCVClient:
    BASE_URL = "https://api.binance.com/api/v3/klines"
    
    def __init__(self):
        self.interval = "5m"
        self.limit = 500
        self.headers = {
            "User-Agent": "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/91.0.4472.124 Safari/537.36"
        }
    
    def get_ohlcv_data(self, symbol: str = "BTCUSDT", limit: Optional[int] = None) -> List[Dict[str, float]]:
        params = {
            "symbol": symbol.upper(),
            "interval": self.interval,
            "limit": min(limit, 1000) if limit else self.limit
        }
        
        try:
            response = requests.get(
                self.BASE_URL,
                params=params,
                headers=self.headers,
                timeout=10
            )
            
            # Check for HTTP errors
            response.raise_for_status()
            
            klines = response.json()
            
            if not isinstance(klines, list):
                raise ValueError(f"Unexpected API response: {klines}")
                
            if not klines:
                print(f"Warning: Empty data returned for {symbol}. Check if the symbol is correct.")
                return []
                
            return [self._format_kline(kline, symbol) for kline in klines]
            
        except Exception as e:
            print(f"Error fetching {symbol} data: {str(e)}")
            return []
    
    def _format_kline(self, kline: List, symbol: str) -> Dict[str, float]:
        """
        Formatea una vela de Binance al esquema deseado.
        """
        open_time = datetime.datetime.utcfromtimestamp(kline[0] / 1000).isoformat() + "Z"
        
        return {
            "begins_at": open_time,
            "open_price": float(kline[1]),
            "high_price": float(kline[2]),
            "low_price": float(kline[3]),
            "close_price": float(kline[4]),
            "volume": float(kline[5]),
            "quote_volume": float(kline[7]),
            "num_trades": int(kline[8]),
            "symbol": symbol,
            "interval": self.interval
        }
    
    def get_latest_candle(self) -> Optional[Dict[str, float]]:
        """Obtiene solo la última vela."""
        data = self.get_ohlcv_data()
        return data[-1] if data else None
