#include <iostream>
#include <sstream>
#include <string>
#include "task_manager.h"
#include "file_utils.h"
#include "ansi.h"

using std::string, std::stringstream, std::endl;
using json = nlohmann::json;

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

bool TaskManager::DeleteAllCompletedTasks() {
    try {
        vector<Task>::iterator it = m_tasks.begin();

        while (it != m_tasks.end()) {
            if (it->GetIsCompleted()) {
                it = m_tasks.erase(it);
            } else {
                it++;
            }
        }
        return true;
    } catch (...) {
        return false;
    }
}

vector<Task> TaskManager::GetTasks() const {
    return m_tasks;
}

void TaskManager::AddTask(const Task &task) {
    m_tasks.push_back(task);
}

bool TaskManager::DeleteTask(const uint &idx, const bool& all, const bool& done) {
    if (all) {
        m_tasks.clear();
        return true;
    }
    if (done) {
        return DeleteAllCompletedTasks();
    }
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

json TaskManager::TasksToJson() const {
    json data = json::object();
    data["tasks"] = json::array();
    for (Task t : m_tasks) {
        json task = json::object();
        task["title"] = t.GetTitle();
        task["complete"] = t.GetIsCompleted();
        data["tasks"].push_back(task);
    }
    return data;
}
