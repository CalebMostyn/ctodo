import os
import shutil
import subprocess
import time

import pytest
import requests

from utils.constants import *

@pytest.fixture(scope="module")
def server(request, task_file):
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

    yield server

    server.kill()
    server.wait()

@pytest.fixture(scope="module")
def task_file(request):
    filepath = request.param

    file_existed_already = False
    if os.path.isfile(filepath):
        file_existed_already = True
        # copy to tmp
        shutil.copy(filepath, f"/tmp/{os.path.basename(filepath)}")

    yield filepath

    if file_existed_already:
        # return file to version prior to test
        shutil.copy(f"/tmp/{os.path.basename(filepath)}", filepath)
    elif os.path.isfile(filepath):
        # delete leftover file
        os.remove(filepath)

