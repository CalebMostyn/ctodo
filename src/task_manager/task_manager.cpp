#include <iostream>
#include <sstream>
#include <string>
#include "task_manager.h"
#include "file_utils.h"
#include "ansi.h"

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

string TaskManager::TasksToString() const {
    stringstream result;
    result << ESC_UNDERLINE << "Tasks:" << ESC_RESET << endl;
    for (size_t ii = 0; ii < m_tasks.size(); ii++) {
        Task t = m_tasks[ii];
        bool is_done = t.GetIsCompleted();

        string done = is_done ? " : X" : "";
        if (!is_done) result << ESC_BOLD;
        result << ii << ". " << t.GetTitle() << done;
        if (!is_done) result << ESC_RESET;
        result << endl;
    }
    return result.str();
}
