# news/sentiment_analyzer.py
from llm.ollama_client import OllamaClient

import os
import json
import threading


class SentimentAnalyzer:
    _file_lock = threading.Lock()
    
    def __init__(self, model):
        self.client = OllamaClient(model=model)

    def analyze_headlines(self, news_items, asset, directory="data/cache"):
        for item in news_items:
            # Clasificar usando 'full_text' en lugar de 'title'
            sentiment = self.client.classify_sentiment(item.get('full_text', ''))
            item['sentiment'] = sentiment

            # Guardar actualización en el archivo correspondiente
            self.update_news_item(item, asset, directory)

        return [item['sentiment'] for item in news_items]


    def aggregate_sentiment(self, news_items, symbol, directory="data/cache"):
        sentiments = self.analyze_headlines(news_items, symbol, directory)
        positive = sentiments.count("positive")
        negative = sentiments.count("negative")
        neutral = sentiments.count("neutral")

        if positive > negative:
            return "positive"
        elif negative > positive:
            return "negative"
        else:
            return "neutral"
        
    def update_news_item(self, item, asset, directory="data/cache"):
        """
        Actualiza un artículo de noticias en el archivo JSON correspondiente si ya existe.
        El artículo se identifica por 'published_date' y 'title'.
        """
        os.makedirs(directory, exist_ok=True)

        pub_date = item['published_date']
        date_str = pub_date.split('T')[0] if 'T' in pub_date else pub_date

        filename = f"{asset}_{date_str}_news.json"
        filepath = os.path.join(directory, filename)

        try:
            with self._file_lock:
                with open(filepath, 'r', encoding='utf-8') as f:
                    existing_data = json.load(f)

                updated = False
                for idx, existing_item in enumerate(existing_data):
                    if (existing_item['published_date'] == item['published_date'] and
                        existing_item['title'] == item['title']):
                        existing_data[idx] = item
                        updated = True
                        break

                if updated:
                    with open(filepath, 'w', encoding='utf-8') as f:
                        json.dump(existing_data, f, indent=4)
                    print(f"[✓] News item updated in {filename}")
                else:
                    print(f"[!] News item not found for update in {filename}")
        except (FileNotFoundError, json.JSONDecodeError) as e:
            print(f"[!] Error updating item: {e}")
