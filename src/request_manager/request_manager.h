#ifndef _REQUEST_MANAGER__H_
#define _REQUEST_MANAGER__H_

#include <memory>

#include "httplib.h"
#include "json.hpp"
#include "task_manager.h"

using std::vector, std::string, std::shared_ptr;
using json = nlohmann::json;
using namespace httplib;

const json INVALID_REQUEST_RESPONSE = {"message", "Invalid Request."};

class RequestManager {
private: 
    shared_ptr<TaskManager> m_task_manager;
public:
    RequestManager(shared_ptr<TaskManager> tm);
    void SetupServer(Server& svr);

    // Requests
    void GetTasks(const Request& req, Response& res);
    void AddTask(const Request& req, Response& res);
    void DeleteTask(const Request& req, Response& res);
    void UpdateTask(const Request& req, Response& res);
};

#endif //_REQUEST_MANAGER__H_
