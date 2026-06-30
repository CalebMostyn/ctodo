import subprocess
import time

import pytest
import requests

from utils.constants import *

@pytest.fixture
def server():
    server = subprocess.Popen([SERVER_BINARY, "-f", "tasks.txt"])
    time.sleep(1)

    response = None
    while not response or response.status_code != 200:
        # check if process died immediately
        if server.poll() is not None:
            raise RuntimeError(
                f"Server exited with code {server.returncode}."
            )
        time.sleep(0.1)
        try:
            response = requests.get(f"{DEFAULT_SERVER_URL}:{DEFAULT_SERVER_PORT}")
        except Exception:
            pass

    yield server

    server.kill()
