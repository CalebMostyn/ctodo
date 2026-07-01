import time
import subprocess

import requests

from utils.constants import *

def start_server(task_file):
    server = subprocess.Popen([SERVER_BINARY, "-f", task_file])

    response = None
    while not response or response.status_code != 200:
        # check if process died immediately
        if server.poll() is not None:
            raise RuntimeError(
                f"Server exited with code {server.returncode}."
            )
        time.sleep(0.1)
        try:
            # send server requests until the default endpoint ('/') responds 200
            response = requests.get(f"{DEFAULT_SERVER_URL}:{DEFAULT_SERVER_PORT}")
        except Exception:
            pass
    return server
