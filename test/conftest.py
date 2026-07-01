import os
import shutil
import time

import pytest
import requests

from utils.server_helpers import kill_all_server_instances, start_server

@pytest.fixture(scope="class")
def server(request, task_file):
    kill_all_server_instances()
    server = start_server(task_file)
    yield server
    server.kill()
    server.wait()

@pytest.fixture(scope="class")
def task_file(request):
    filepath = request.param

    file_existed_already = False
    if os.path.isfile(filepath):
        file_existed_already = True
        # copy to tmp
        shutil.copy(filepath, f"/tmp/{os.path.basename(filepath)}")

    yield filepath

    try:
        assert os.path.isfile(filepath), f"Test file {filepath} does not exist."
    finally:
        if file_existed_already:
            # return file to version prior to test
            shutil.copy(f"/tmp/{os.path.basename(filepath)}", filepath)
        elif os.path.isfile(filepath):
            # delete leftover file
            os.remove(filepath)

