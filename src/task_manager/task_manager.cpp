#include <iostream>
#include <sstream>
#include <string>
#include "task_manager.h"
#include "file_utils.h"

using std::string, std::stringstream, std::endl;

TaskManager::TaskManager(string file_path) {
    std::cout << "making task manager with file" << std::endl;
    m_file_path = file_path;
    FileUtils::ParseTasksFile(m_file_path, *this);
}

TaskManager::~TaskManager() {
    std::cout << "destructing" << endl;
    FileUtils::WriteTasksFile(m_file_path, *this);
}

vector<Task> TaskManager::GetTasks() const {
    return m_tasks;
}

void TaskManager::AddTask(const Task &task) {
    m_tasks.push_back(task);
}

bool TaskManager::DeleteTask(const uint &idx) {
    if (idx < m_tasks.size()) {
        m_tasks.erase(m_tasks.begin() + idx);
        return true;
    } else {
        return false;
    }
}

string TaskManager::TasksToString() const {
    stringstream result;
    result << "Tasks:" << endl;
    for (size_t ii = 0; ii < m_tasks.size(); ii++) {
        Task t = m_tasks[ii];
        string done = t.GetIsCompleted() ? "done" : "not done";
        result << ii << ". " << t.GetTitle() << " : " << done << endl;
    }
    return result.str();
}
