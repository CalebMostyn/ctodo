#ifndef _TASK_MANAGER__H_
#define _TASK_MANAGER__H_

#include <vector>
#include <string>
#include "task.h"

using std::vector, std::string;

class TaskManager {
private:
    vector<Task> m_tasks;
    string m_file_path;
    bool ValidTaskIdx(const uint& idx);
public:
    TaskManager(string file_path);
    ~TaskManager();

    vector<Task> GetTasks() const;
    void AddTask(const Task& task);
    bool DeleteTask(const uint& idx);
    bool UpdateTask(const uint& idx, const string& title, const bool& status);
    bool UpdateTask(const uint& idx, const bool& status);
    bool UpdateTask(const uint& idx, const string& title);

    string TasksToString() const;
};

#endif //_TASK_MANAGER__H_
