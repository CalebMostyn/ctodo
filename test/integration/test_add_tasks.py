import time

import pytest
import requests

from utils.constants import *

@pytest.mark.parametrize("task_file", ["test_files/blank_tasks.json"], indirect=True)
class TestAddTasks:

    test_task_title = "test task : "

    def format_task_title(self, num):
        return f'{self.test_task_title}{num}'

    # add single task successfully
    def test_add_task(self, task_file, server):
        response = requests.get(f"{DEFAULT_SERVER_URL}:{DEFAULT_SERVER_PORT}/tasks")
        assert response.status_code == 200
        tasks = response.json()["tasks"]
        assert len(tasks) == 0, f"Expected empty task list, got {len(tasks)}."

        response = requests.post(f"{DEFAULT_SERVER_URL}:{DEFAULT_SERVER_PORT}/tasks", data={"title": self.format_task_title(0)})
        assert response.status_code == 200

        response = requests.get(f"{DEFAULT_SERVER_URL}:{DEFAULT_SERVER_PORT}/tasks")
        assert response.status_code == 200
        tasks = response.json()["tasks"]
        assert len(tasks) == 1, f"Expected task list with 1 entry, got {len(tasks)}."
        assert tasks[0]["title"] == self.format_task_title(0), f"Task has incorrect title, expected {self.format_task_title(0)}, got {tasks[0]["title"]}."
        assert tasks[0]["complete"] == False, f"Newly created task is marked as complete."

    # fail to add task with no title
    def test_add_task_no_title(self, task_file, server):
        response = requests.post(f"{DEFAULT_SERVER_URL}:{DEFAULT_SERVER_PORT}/tasks")
        assert response.status_code == 400

        response = requests.get(f"{DEFAULT_SERVER_URL}:{DEFAULT_SERVER_PORT}/tasks")
        tasks = response.json()["tasks"]
        assert len(tasks) == 1, f"Expected task list with 1 entry, got {len(tasks)}."

    # add 1000 tasks
    def test_add_several_tasks(self, task_file, server):
        start_ts = time.monotonic()
        for ii in range(1, 1000):
            response = requests.post(f"{DEFAULT_SERVER_URL}:{DEFAULT_SERVER_PORT}/tasks", data={"title": self.format_task_title(ii)})
            assert response.status_code == 200
        delta_t = (time.monotonic() - start_ts)
        assert delta_t < 1, f"1000 post requests took longer than 1 second."

        response = requests.get(f"{DEFAULT_SERVER_URL}:{DEFAULT_SERVER_PORT}/tasks")
        assert response.status_code == 200
        tasks = response.json()["tasks"]
        assert len(tasks) == 1000, f"Expected task list with 1000 entries, got {len(tasks)}."
        for ii in range(len(tasks)):
            assert tasks[ii]["title"] == self.format_task_title(ii), f"Task has incorrect title, expected {self.format_task_title(ii)}, got {tasks[ii]["title"]}."
            assert tasks[ii]["complete"] == False, f"Newly created task is marked as complete."

