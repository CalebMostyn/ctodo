#include "request_manager.h"
#include <iostream>

RequestManager::RequestManager(shared_ptr<TaskManager> tm) {
    m_task_manager = tm;
}

void RequestManager::SetupServer(Server& svr) {
    svr.Get("/tasks", [this](const Request& req, Response& res) {
        GetTasks(req, res);
    });
    svr.Post("/tasks", [this](const Request& req, Response& res) {
        AddTask(req, res);
    });
    svr.Delete("/tasks", [this](const Request& req, Response& res) {
        DeleteTask(req, res);
    });
}

void RequestManager::GetTasks(const Request& req, Response& res) {
    res.set_content(m_task_manager->TasksToString(), "text/plain");
}

void RequestManager::AddTask(const Request& req, Response& res) {
    m_task_manager->AddTask(req.get_param_value("title"));
    res.set_content("Task Added Successfully.\n", "text/plain");
}

void RequestManager::DeleteTask(const Request& req, Response& res) {
    uint task_idx;
    try {
        task_idx = std::stoi(req.get_param_value("task"));
    } catch (...) {
        res.status = 400;
        res.set_content("Invalid Request.\n", "text/plain");
        return;
    }

    if (m_task_manager->DeleteTask(task_idx)) {
        res.set_content("Task Deleted Successfully.\n", "text/plain");
    } else {
        res.status = 404;
        res.set_content("Task Does not Exist.\n", "text/plain");
    }
}
