
import threading


class Logger:
    _print_lock = threading.Lock()

    def __init__(self):
        pass

    @staticmethod
    def debug_print(msg):
        with Logger._print_lock:
            print(f"[DEBUG] {msg}")