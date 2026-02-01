#include <sstream>
#include <fstream>
#include <string>
#include <stdexcept>
#include "file_utils.h"

using std::stringstream, std::endl, std::string;

void FileUtils::ParseTasksFile(string file_path, TaskManager &tm) {
    std::ifstream ifs(file_path);
    if (!ifs.is_open()) {
        stringstream err;
        err << "Unable to open file " << file_path << endl;
        throw std::runtime_error(err.str());
    }

    string line;
    bool found_tasks_header = false;
    while (std::getline(ifs, line)) {
        if (line == TASKS_HEADER) {
            found_tasks_header = true;
            continue;
        }
        if (found_tasks_header) {
            Task* t = new Task(line);
            if (line.substr(line.length() - 2) == " X" || line.substr(line.length() - 2) == " x") {
                t->SetIsCompleted(true);
                string substr = line.substr(0, line.find_last_of(' '));
                t->SetTitle(substr);
            }
            tm.AddTask(*t);
        }
    }
}

void FileUtils::WriteTasksFile(string file_path, const TaskManager &tm) {
    std::ofstream ofs(file_path);
    if (!ofs.is_open()) {
        stringstream err;
        err << "Unable to open file " << file_path << endl;
        throw std::runtime_error(err.str());
    }

    ofs << TASKS_HEADER << endl;
    for (Task t : tm.GetTasks()) {
        string done = t.GetIsCompleted() ? " x" : "";
        ofs << t.GetTitle() << done << endl;
    }
}

