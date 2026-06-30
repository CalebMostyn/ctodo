import pytest
import requests

from utils.constants import *

@pytest.mark.parametrize("task_file", ["test_files/blank_tasks.json", "nonexistent_tasks.json"], indirect=True)
class TestTasks:

    task_file = "blank_tasks.txt"
    test_task_title = "test task"

    def test_get_blank_tasks(self, task_file, server):
        response = requests.get(f"{DEFAULT_SERVER_URL}:{DEFAULT_SERVER_PORT}/tasks")
        assert response.status_code == 200
        tasks = response.json()["tasks"]
        assert len(tasks) == 0, f"Expected empty task list, got {len(tasks)}."

    def test_add_task(self, task_file, server):
        response = requests.post(f"{DEFAULT_SERVER_URL}:{DEFAULT_SERVER_PORT}/tasks", data={"title": self.test_task_title})
        assert response.status_code == 200

    def test_get_added_task(self, task_file, server):
        response = requests.get(f"{DEFAULT_SERVER_URL}:{DEFAULT_SERVER_PORT}/tasks")
        assert response.status_code == 200
        tasks = response.json()["tasks"]
        assert len(tasks) == 1, f"Expected task list with 1 entry, got {len(tasks)}."
        assert tasks[0]["title"] == self.test_task_title, f"Task has incorrect title, expected {self.test_task_title}, got {tasks[0]["title"]}."
        assert tasks[0]["complete"] == False, f"Newly created task is marked as complete."

