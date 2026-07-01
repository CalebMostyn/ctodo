import time
import subprocess
import psutil
import getpass

import pytest
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

# kills server instances owned by this user, if ran by other user
# such as sudo by systemd, tests will exit, as they may interfere
def kill_all_server_instances():
    ps_name = 'ctodo-server'
    for proc in psutil.process_iter(["pid", "name", "cmdline"]):
        try:
            name = proc.info["name"] or ""
            cmdline = " ".join(proc.info["cmdline"] or [])

            if ps_name in name.lower() or ps_name in cmdline.lower():
                if proc.username() != getpass.getuser():
                    pytest.exit("Non-owned server process running, exiting tests.")
                else:
                    # sigterm for graceful shutdown
                    proc.terminate()
        except (psutil.NoSuchProcess, psutil.AccessDenied, psutil.ZombieProcess):
            pass

