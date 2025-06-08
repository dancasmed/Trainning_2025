# llm/ollama_client.py
import requests
import json
import time
import random
from datetime import datetime, timedelta

from utils.thermal_helper import get_thermal_pressure_level

class OllamaClient:
    def __init__(self, model, max_retries=3, max_retry_delay=15, timeout=60):
        self.model = model
        self.url = "http://localhost:11434/api/generate"
        self.max_retries = max_retries
        self.max_retry_delay = max_retry_delay
        self.timeout = timeout  # segundos

    def generate(self, prompt):
        payload = {
            "model": self.model,
            "prompt": prompt,
            "stream": False
        }

        for attempt in range(1, self.max_retries + 1):
            get_thermal_pressure_level()
            try:
                response = requests.post(self.url, json=payload, timeout=self.timeout)
                if response.status_code == 200:
                    return json.loads(response.text)['response']
                else:
                    print(f"[!] Attempt {attempt} failed with status code {response.status_code}: {response.text}")
            except (requests.ConnectionError, requests.Timeout) as e:
                print(f"[!] Connection error on attempt {attempt}: {str(e)}")
            
            if attempt < self.max_retries:
                current_delay = random.randint(5, self.max_retry_delay) * attempt  # Incremental backoff
                print(f"Retrying in {current_delay} seconds...")
                time.sleep(current_delay)

        raise Exception("Failed to get response from Ollama after multiple attempts")

    def classify_sentiment(self, text):
        prompt = (
            "Classify the following financial news article as either "
            "'positive', 'neutral', or 'negative':\n\n" + text[:2000]  # Limitar texto
        )
        result = self.generate(prompt).lower()
        if "positive" in result:
            return "positive"
        elif "negative" in result:
            return "negative"
        else:
            return "neutral"

    def generate_search_query(self, asset):
        today = datetime.utcnow().date()
        start_date = today - timedelta(days=7)
        after = start_date.isoformat()
        before = (today + timedelta(days=1)).isoformat()

        prompt = f"""
    Generate a concise and highly optimized Google News search query to find headlines published after:{after} and before:{before} that could influence the price or market sentiment of {asset} crypto.

    The query should prioritize news related to:
    - Regulatory changes or government actions
    - Institutional adoption or large investments
    - ETF approvals or financial product launches
    - Market crashes, volatility, or macroeconomic events
    - Energy consumption, mining activity, or network upgrades
    - Geopolitical events affecting crypto markets
    - Major partnerships or technological advancements
    - Security breaches or hacks
    - Market sentiment shifts or public perception changes
    - Must be sure the context is related to the crypto market.

    Only return the final search query string. No explanations.
    """
        result = self.generate(prompt).strip()
        return result