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
public:
    TaskManager();
    TaskManager(string file_path);
    ~TaskManager();

    vector<Task> GetTasks() const;
    void AddTask(const Task &task);
    string TasksToString() const;
};

#endif //_TASK_MANAGER__H_
