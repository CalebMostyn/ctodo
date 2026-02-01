#include <iostream>
#include <sstream>
#include <string>
#include "task_manager.h"
#include "file_utils.h"

using std::string, std::stringstream, std::endl;

TaskManager::TaskManager(string file_path) {
    m_file_path = file_path;
    FileUtils::ParseTasksFile(m_file_path, *this);
}

TaskManager::~TaskManager() {
    FileUtils::WriteTasksFile(m_file_path, *this);
}

bool TaskManager::ValidTaskIdx(const uint& idx) {
    if (idx < m_tasks.size()) {
        return true;
    } else {
        return false;
    }
}

vector<Task> TaskManager::GetTasks() const {
    return m_tasks;
}

void TaskManager::AddTask(const Task &task) {
    m_tasks.push_back(task);
}

bool TaskManager::DeleteTask(const uint &idx) {
    if (ValidTaskIdx(idx)) {
        m_tasks.erase(m_tasks.begin() + idx);
        return true;
    } 

    return false;
}
bool TaskManager::UpdateTask(const uint& idx, const string& title, const bool& status) {
    if (ValidTaskIdx(idx)) {
        Task* t = &m_tasks[idx];
        t->SetTitle(title);
        t->SetIsCompleted(status);
        return true;
    }
    return false;
}

bool TaskManager::UpdateTask(const uint& idx, const bool& status) {
    if (ValidTaskIdx(idx)) {
        Task* t = &m_tasks[idx];
        t->SetIsCompleted(status);
        return true;
    }
    return false;
}

bool TaskManager::UpdateTask(const uint& idx, const string& title) {
    if (ValidTaskIdx(idx)) {
        Task* t = &m_tasks[idx];
        t->SetTitle(title);
        return true;
    }
    return false;
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
