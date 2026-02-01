#ifndef _REQUEST_MANAGER__H_
#define _REQUEST_MANAGER__H_

#include "httplib.h"
#include "task_manager.h"

using std::vector, std::string;
using namespace httplib;

class RequestManager {
private: 
    TaskManager m_task_manager;
public:
    RequestManager(TaskManager& tm);
    void SetupServer(Server& svr);

    // Requests
    void GetTasks(const Request& req, Response& res);
};

#endif //_REQUEST_MANAGER__H_
