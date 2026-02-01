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
    svr.Patch("/tasks", [this](const Request& req, Response& res) {
        UpdateTask(req, res);
    });
}

void RequestManager::GetTasks(const Request& req, Response& res) {
    res.set_content(m_task_manager->TasksToString(), "text/plain");
}

void RequestManager::AddTask(const Request& req, Response& res) {
    string task_title;
    try {
        task_title = req.get_param_value("title");
    } catch (...) {
        res.status = 400;
        res.set_content("Invalid Request.\n", "text/plain");
        return;
    }

    m_task_manager->AddTask(task_title);
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
        res.set_content("Task Does Not Exist.\n", "text/plain");
    }
}

void RequestManager::UpdateTask(const Request& req, Response& res) {
    uint task_idx;

    bool has_title = false;
    string new_title;
    bool has_status = false;
    bool new_status;
    try {
        task_idx = std::stoi(req.get_param_value("task"));

        string title_param = req.get_param_value("title");
        if (title_param != "") {
            has_title = true;
            new_title = title_param;
        }

        string status_param = req.get_param_value("done");
        if (status_param != "") {
            std::transform(status_param.begin(), status_param.end(), status_param.begin(), ::tolower);
            if (status_param == "true" || status_param == "1") {
                has_status = true;
                new_status = true;
            } else if (status_param == "false" || status_param == "0") {
                has_status = true;
                new_status = false;
            }
        }

        if (!has_title && !has_status) {
            throw std::invalid_argument("No Parameters to Change.");
        }
    } catch (...) {
        res.status = 400;
        res.set_content("Invalid Request.\n", "text/plain");
        return;
    }

    bool success = false;
    if (has_title && has_status) {
        success = m_task_manager->UpdateTask(task_idx, new_title, new_status);
    } else if (has_title) {
        success = m_task_manager->UpdateTask(task_idx, new_title);
    } else if (has_status) {
        success = m_task_manager->UpdateTask(task_idx, new_status);
    }

    if (success) {
        res.set_content("Task Updated Successfully.\n", "text/plain");
    } else {
        res.status = 404;
        res.set_content("Task Does Not Exist.\n", "text/plain");
    }
}
