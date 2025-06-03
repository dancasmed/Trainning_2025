# llm/ollama_client.py
import requests
import json

from datetime import datetime, timedelta

class OllamaClient:
    def __init__(self, model):
        self.model = model
        self.url = "http://localhost:11434/api/generate"

    def generate(self, prompt):
        payload = {
            "model": self.model,
            "prompt": prompt,
            "stream": False
        }
        response = requests.post(self.url, json=payload)
        if response.status_code == 200:
            return json.loads(response.text)['response']
        else:
            raise Exception(f"Ollama error: {response.text}")

    def classify_sentiment(self, text):
        prompt = (
            "Classify the following financial news article as either "
            "'positive', 'neutral', or 'negative':\n\n" + text
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
        
        # Format dates in YYYY-MM-DD for compatibility with search syntax
        after = start_date.isoformat()
        before = (today + timedelta(days=1)).isoformat()  # To include today's full day

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
