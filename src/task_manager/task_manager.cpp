#include <sstream>
#include <string>
#include "task_manager.h"
#include "file_utils.h"

using std::string, std::stringstream, std::endl;

TaskManager::TaskManager() { }

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
