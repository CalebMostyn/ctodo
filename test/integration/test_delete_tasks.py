import pytest
import requests

from utils.constants import *

@pytest.mark.parametrize("task_file", ["test_files/five_tasks.json"], indirect=True)
class TestDeleteTasks:

    test_task_title = "test : "

    def format_task_title(self, num):
        return f'{self.test_task_title}{num}'

    # delete task succesfully
    def test_delete_task(self, task_file, server):
        response = requests.get(f"{DEFAULT_SERVER_URL}:{DEFAULT_SERVER_PORT}/tasks")
        assert response.status_code == 200
        tasks = response.json()["tasks"]
        assert len(tasks) == 5, f"Expected task list with 5 entries, got {len(tasks)}."

        response = requests.delete(f"{DEFAULT_SERVER_URL}:{DEFAULT_SERVER_PORT}/tasks", data={"task": 0})
        assert response.status_code == 200

        response = requests.get(f"{DEFAULT_SERVER_URL}:{DEFAULT_SERVER_PORT}/tasks")
        assert response.status_code == 200
        tasks = response.json()["tasks"]
        assert len(tasks) == 4, f"Expected task list with 4 entries, got {len(tasks)}."

        # should include tasks 1-4 still
        for ii in range(4):
            assert tasks[ii]['title'] == self.format_task_title(ii + 1), f"Unexpected task title: expected {self.format_task_title(ii + 1)}, got {tasks[ii]['title']}"

    # fail on delete with invalid task index
    def test_delete_nonexistent_task(self, task_file, server):
        response = requests.delete(f"{DEFAULT_SERVER_URL}:{DEFAULT_SERVER_PORT}/tasks", data={"task": 999})
        assert response.status_code == 404

    # delete just the tasks marked as done
    def test_clear_done(self, task_file, server):
        response = requests.delete(f"{DEFAULT_SERVER_URL}:{DEFAULT_SERVER_PORT}/tasks", data={"task": "done"})
        assert response.status_code == 200

        response = requests.get(f"{DEFAULT_SERVER_URL}:{DEFAULT_SERVER_PORT}/tasks")
        assert response.status_code == 200
        tasks = response.json()["tasks"]
        assert len(tasks) == 2, f"Expected task list with 2 entries, got {len(tasks)}."
        assert tasks[0]['title'] == self.format_task_title(2), f"Unexpected task title: expected {self.format_task_title(2)}, got {tasks[0]['title']}"
        assert tasks[1]['title'] == self.format_task_title(4), f"Unexpected task title: expected {self.format_task_title(4)}, got {tasks[1]['title']}"

    # delete all tasks, regardless of done status
    def test_clear_all(self, task_file, server):
        # add and mark as done some tasks to include both complete and incomplete tasks
        response = requests.post(f"{DEFAULT_SERVER_URL}:{DEFAULT_SERVER_PORT}/tasks", data={"title": self.format_task_title(0)})
        assert response.status_code == 200
        response = requests.patch(f"{DEFAULT_SERVER_URL}:{DEFAULT_SERVER_PORT}/tasks", data={"task": 2,"done": True})
        assert response.status_code == 200
        response = requests.post(f"{DEFAULT_SERVER_URL}:{DEFAULT_SERVER_PORT}/tasks", data={"title": self.format_task_title(1)})
        assert response.status_code == 200
        response = requests.patch(f"{DEFAULT_SERVER_URL}:{DEFAULT_SERVER_PORT}/tasks", data={"task": 3,"done": True})
        assert response.status_code == 200

        response = requests.get(f"{DEFAULT_SERVER_URL}:{DEFAULT_SERVER_PORT}/tasks")
        assert response.status_code == 200
        tasks = response.json()["tasks"]
        assert len(tasks) == 4, f"Expected task list with 4 entries, got {len(tasks)}."

        response = requests.delete(f"{DEFAULT_SERVER_URL}:{DEFAULT_SERVER_PORT}/tasks", data={"task": "*"})
        assert response.status_code == 200

        response = requests.get(f"{DEFAULT_SERVER_URL}:{DEFAULT_SERVER_PORT}/tasks")
        assert response.status_code == 200
        tasks = response.json()["tasks"]
        assert len(tasks) == 0, f"Expected empty task list, got {len(tasks)}."
