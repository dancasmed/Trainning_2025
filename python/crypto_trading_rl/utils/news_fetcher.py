# utils/news_fetcher.py
import feedparser
from urllib.parse import quote_plus
from typing import List, Dict


def fetch_news_from_rss(query: str, limit: int = 5) -> List[Dict]:
    """
    Busca noticias en Google News RSS usando una consulta dada.
    Devuelve una lista de artículos con título, resumen y fecha.
    """
    encoded_query = quote_plus(query)
    rss_url = f"https://news.google.com/rss/search?q={encoded_query}&hl=en&gl=US&ceid=US:en"

    print(f"[+] Fetching news from: {rss_url}")

    feed = feedparser.parse(rss_url)
    entries = []

    for i, entry in enumerate(feed.entries):
        if i >= limit:
            break
        entries.append({
            "title": entry.title,
            "summary": entry.summary,
            "link": entry.link,
            "published": entry.published
        })

    print(f"[✓] Retrieved {len(entries)} news articles.")
    return entries