import json
import signal

import pytest
import requests

from utils.constants import *
from utils.server_helpers import start_server

@pytest.mark.parametrize("task_file", ["test_files/blank_tasks.json", "nonexistent_tasks.json"], indirect=True)
class TestSigint:

    test_task_title = "test task"

    # ensure ctrl-c gracefully shutsdown and saves taskfile on exit
    def test_sigint(self, task_file, server):
        response = requests.post(f"{DEFAULT_SERVER_URL}:{DEFAULT_SERVER_PORT}/tasks", data={"title": self.test_task_title})
        assert response.status_code == 200

        response = requests.get(f"{DEFAULT_SERVER_URL}:{DEFAULT_SERVER_PORT}/tasks")
        assert response.status_code == 200
        tasks = response.json()["tasks"]
        assert len(tasks) == 1, f"Expected task list with 1 entry, got {len(tasks)}."
        assert tasks[0]["title"] == self.test_task_title, f"Task has incorrect title, expected {self.test_task_title}, got {tasks[0]["title"]}."
        assert tasks[0]["complete"] == False, f"Newly created task is marked as complete."

        response = requests.patch(f"{DEFAULT_SERVER_URL}:{DEFAULT_SERVER_PORT}/tasks", data={"task": 0, "done": True})
        assert response.status_code == 200

        response = requests.get(f"{DEFAULT_SERVER_URL}:{DEFAULT_SERVER_PORT}/tasks")
        tasks = response.json()["tasks"]
        assert len(tasks) == 1, f"Expected task list with 1 entry, got {len(tasks)}."
        assert tasks[0]["title"] == self.test_task_title, f"Task has incorrect title, expected {self.test_task_title}, got {tasks[0]["title"]}."
        assert tasks[0]["complete"] == True, f"Failed to mark task as complete."

        server.send_signal(signal.SIGINT)
        exit_code = server.wait()
        assert exit_code == 0, f"Error on exit, returned code {exit_code}."

        # read task file that should have saved on exit
        with open(task_file, 'r') as file:
            data = json.load(file)

        assert len(data['tasks']) == 1, f"Expected task file with list of length 1, got {len(data['tasks'])}"
        assert data['tasks'][0]["title"] == self.test_task_title, f"Saved task has incorrect title, expected {self.test_task_title}, got {data['tasks'][0]["title"]}."
        assert data['tasks'][0]["complete"] == True, f"Saved task is not marked as complete."

    # ensure process can be restarted with same task file and then killed
    def test_reopen(self, task_file):
        server = start_server(task_file)

        response = requests.get(f"{DEFAULT_SERVER_URL}:{DEFAULT_SERVER_PORT}/tasks")
        tasks = response.json()["tasks"]
        assert len(tasks) == 1, f"Expected task list with 1 entry, got {len(tasks)}."
        assert tasks[0]["title"] == self.test_task_title, f"Task has incorrect title, expected {self.test_task_title}, got {tasks[0]["title"]}."
        assert tasks[0]["complete"] == True, f"Failed to mark task as complete."

        server.kill()
        exit_code = server.wait()
        assert exit_code == -9, f"Error on kill, returned code {exit_code} instead of -9."


@pytest.mark.parametrize("task_file", ["test_files/blank_tasks.json", "nonexistent_tasks.json"], indirect=True)
class TestSigterm:

    test_task_title = "test task"

    # ensure terminate gracefully exits
    def test_sigterm(self, task_file, server):
        response = requests.post(f"{DEFAULT_SERVER_URL}:{DEFAULT_SERVER_PORT}/tasks", data={"title": self.test_task_title})
        assert response.status_code == 200

        response = requests.get(f"{DEFAULT_SERVER_URL}:{DEFAULT_SERVER_PORT}/tasks")
        assert response.status_code == 200
        tasks = response.json()["tasks"]
        assert len(tasks) == 1, f"Expected task list with 1 entry, got {len(tasks)}."
        assert tasks[0]["title"] == self.test_task_title, f"Task has incorrect title, expected {self.test_task_title}, got {tasks[0]["title"]}."
        assert tasks[0]["complete"] == False, f"Newly created task is marked as complete."

        response = requests.patch(f"{DEFAULT_SERVER_URL}:{DEFAULT_SERVER_PORT}/tasks", data={"task": 0, "done": True})
        assert response.status_code == 200

        response = requests.get(f"{DEFAULT_SERVER_URL}:{DEFAULT_SERVER_PORT}/tasks")
        tasks = response.json()["tasks"]
        assert len(tasks) == 1, f"Expected task list with 1 entry, got {len(tasks)}."
        assert tasks[0]["title"] == self.test_task_title, f"Task has incorrect title, expected {self.test_task_title}, got {tasks[0]["title"]}."
        assert tasks[0]["complete"] == True, f"Failed to mark task as complete."

        server.terminate()
        exit_code = server.wait()
        assert exit_code == 0, f"Error on exit, returned code {exit_code}."

        # read task file that should have saved on exit
        with open(task_file, 'r') as file:
            data = json.load(file)

        assert len(data['tasks']) == 1, f"Expected task file with list of length 1, got {len(data['tasks'])}"
        assert data['tasks'][0]["title"] == self.test_task_title, f"Saved task has incorrect title, expected {self.test_task_title}, got {data['tasks'][0]["title"]}."
        assert data['tasks'][0]["complete"] == True, f"Saved task is not marked as complete."

    # ensure process can be restarted with same task file and then killed
    def test_reopen(self, task_file):
        server = start_server(task_file)

        response = requests.get(f"{DEFAULT_SERVER_URL}:{DEFAULT_SERVER_PORT}/tasks")
        tasks = response.json()["tasks"]
        assert len(tasks) == 1, f"Expected task list with 1 entry, got {len(tasks)}."
        assert tasks[0]["title"] == self.test_task_title, f"Task has incorrect title, expected {self.test_task_title}, got {tasks[0]["title"]}."
        assert tasks[0]["complete"] == True, f"Failed to mark task as complete."

        server.kill()
        exit_code = server.wait()
        assert exit_code == -9, f"Error on kill, returned code {exit_code} instead of -9."

@pytest.mark.parametrize("task_file", ["test_files/blank_tasks.json", "nonexistent_tasks.json"], indirect=True)
class TestSighup:

    test_task_title = "test task"

    # ensure sighup gracefully exits
    def test_sigterm(self, task_file, server):
        response = requests.post(f"{DEFAULT_SERVER_URL}:{DEFAULT_SERVER_PORT}/tasks", data={"title": self.test_task_title})
        assert response.status_code == 200

        response = requests.get(f"{DEFAULT_SERVER_URL}:{DEFAULT_SERVER_PORT}/tasks")
        assert response.status_code == 200
        tasks = response.json()["tasks"]
        assert len(tasks) == 1, f"Expected task list with 1 entry, got {len(tasks)}."
        assert tasks[0]["title"] == self.test_task_title, f"Task has incorrect title, expected {self.test_task_title}, got {tasks[0]["title"]}."
        assert tasks[0]["complete"] == False, f"Newly created task is marked as complete."

        response = requests.patch(f"{DEFAULT_SERVER_URL}:{DEFAULT_SERVER_PORT}/tasks", data={"task": 0, "done": True})
        assert response.status_code == 200

        response = requests.get(f"{DEFAULT_SERVER_URL}:{DEFAULT_SERVER_PORT}/tasks")
        tasks = response.json()["tasks"]
        assert len(tasks) == 1, f"Expected task list with 1 entry, got {len(tasks)}."
        assert tasks[0]["title"] == self.test_task_title, f"Task has incorrect title, expected {self.test_task_title}, got {tasks[0]["title"]}."
        assert tasks[0]["complete"] == True, f"Failed to mark task as complete."

        server.send_signal(signal.SIGHUP)
        exit_code = server.wait()
        assert exit_code == 0, f"Error on exit, returned code {exit_code}."

        # read task file that should have saved on exit
        with open(task_file, 'r') as file:
            data = json.load(file)

        assert len(data['tasks']) == 1, f"Expected task file with list of length 1, got {len(data['tasks'])}"
        assert data['tasks'][0]["title"] == self.test_task_title, f"Saved task has incorrect title, expected {self.test_task_title}, got {data['tasks'][0]["title"]}."
        assert data['tasks'][0]["complete"] == True, f"Saved task is not marked as complete."

    # ensure process can be restarted with same task file and then killed
    def test_reopen(self, task_file):
        server = start_server(task_file)

        response = requests.get(f"{DEFAULT_SERVER_URL}:{DEFAULT_SERVER_PORT}/tasks")
        tasks = response.json()["tasks"]
        assert len(tasks) == 1, f"Expected task list with 1 entry, got {len(tasks)}."
        assert tasks[0]["title"] == self.test_task_title, f"Task has incorrect title, expected {self.test_task_title}, got {tasks[0]["title"]}."
        assert tasks[0]["complete"] == True, f"Failed to mark task as complete."

        server.kill()
        exit_code = server.wait()
        assert exit_code == -9, f"Error on kill, returned code {exit_code} instead of -9."

