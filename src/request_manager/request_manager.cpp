#include "request_manager.h"

RequestManager::RequestManager(TaskManager& tm) {
    m_task_manager = tm;
}

void RequestManager::SetupServer(Server& svr) {
    svr.Get("/tasks", [this](const Request& req, Response& res) {
        GetTasks(req, res);
    });
}

void RequestManager::GetTasks(const Request& req, Response& res) {
    res.set_content(m_task_manager.TasksToString(), "text/plain");
}
