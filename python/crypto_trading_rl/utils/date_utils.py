from datetime import datetime, timedelta

def generate_expected_intervals(interval_seconds: int, start_date: datetime, end_date: datetime) -> set:
    expected_times = set()
    current = start_date
    while current < end_date:
        expected_times.add(current.strftime("%Y-%m-%d %H:%M"))
        current += timedelta(seconds=interval_seconds)
    return expected_times