#include <string>
#include "task_manager.h"
#include "file_utils.h"

using std::string;

TaskManager::TaskManager(string file_path) {
    m_file_path = file_path;
    FileUtils::ParseTasksFile(m_file_path, *this);
}

TaskManager::~TaskManager() {
    FileUtils::WriteTasksFile(m_file_path, *this);
}

vector<Task> TaskManager::GetTasks() const {
    return m_tasks;
}

void TaskManager::AddTask(const Task &task) {
    m_tasks.push_back(task);
}
