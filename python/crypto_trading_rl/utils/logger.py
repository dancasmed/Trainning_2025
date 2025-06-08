
import threading


class Logger:
    _print_lock = threading.Lock()
    _log_level = "DEBUG"  # Default log level

    def __init__(self):
        pass

    @staticmethod
    def debug_print(msg):
        with Logger._print_lock:
            if Logger._log_level == "DEBUG":
                print(f"[DEBUG] {msg}")
                
    @staticmethod
    def info_print(msg):
        with Logger._print_lock:
            if Logger._log_level == "INFO" or Logger._log_level == "DEBUG":
                print(f"[INFO] {msg}")
                
    @staticmethod
    def error_print(msg):
        with Logger._print_lock:
            if Logger._log_level == "ERROR" or Logger._log_level == "INFO" or Logger._log_level == "DEBUG":
                print(f"[ERROR] {msg}")

    @staticmethod
    def set_log_level(level: str):
        Logger._log_level = level

    @staticmethod
    def get_log_level() -> str:
        return Logger._log_level