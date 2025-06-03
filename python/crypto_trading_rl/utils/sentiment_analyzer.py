# utils/sentiment_analyzer.py
import ollama


def generate_news_search_prompt(asset: str, model:str) -> str:
    """
    Genera una consulta de búsqueda optimizada para Google News.
    """
    prompt = f"""
Escribe una consulta de búsqueda optimizada para encontrar noticias recientes sobre "{asset}".
La consulta debe ser breve, clara y enfocada en actualidad.
"""

    response = ollama.generate(model, prompt=prompt)
    return response["response"].strip()


def analyze_news_sentiment(news_text: str, model:str) -> str:
    """
    Analiza el sentimiento de un texto de noticia.
    Devuelve: POSITIVE / NEGATIVE / NEUTRAL
    """
    prompt = f"""
Clasifica el sentimiento del siguiente texto de noticia: "{news_text}"
Devuelve únicamente uno de los siguientes valores: POSITIVE, NEGATIVE o NEUTRAL.
"""

    response = ollama.generate(model, prompt=prompt)
    sentiment = response["response"].strip().upper()

    if sentiment not in ["POSITIVE", "NEGATIVE", "NEUTRAL"]:
        return "NEUTRAL"  # Fallback
    return sentiment