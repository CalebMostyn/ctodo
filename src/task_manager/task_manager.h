#ifndef _TASK_MANAGER__H_
#define _TASK_MANAGER__H_

#include "json.hpp"
#include "task.h"

#include <vector>
#include <string>
#include <mutex>

using std::vector, std::string, std::mutex;
using json = nlohmann::json;

class TaskManager {
private:
    vector<Task> m_tasks;
    string m_file_path;
    mutex m_tasks_mutex;
    bool ValidTaskIdx(const uint& idx);
    bool DeleteAllCompletedTasks();
public:
    TaskManager(string file_path);

    vector<Task> GetTasks();
    void AddTask(const Task& task);
    bool DeleteTask(const uint& idx, const bool& all, const bool& done);
    bool UpdateTask(const uint& idx, const string& title, const bool& status);
    bool UpdateTask(const uint& idx, const bool& status);
    bool UpdateTask(const uint& idx, const string& title);

    json TasksToJson();
    void Save();
};

#endif //_TASK_MANAGER__H_
