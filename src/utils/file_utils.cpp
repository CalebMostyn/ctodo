#include <sstream>
#include <fstream>
#include <filesystem>
#include <string>
#include <stdexcept>
#include <iostream>
#include "json.hpp"
#include "file_utils.h"

using std::stringstream, std::endl, std::string, std::cout;
using json = nlohmann::json;

void FileUtils::ParseTasksFile(string file_path, TaskManager &tm) {
    
    std::ifstream ifs(file_path);
    if (!ifs.is_open()) {
        // Create file if does not exist
        WriteTasksFile(file_path, tm);
    }
    json data = json::parse(ifs);

    for (auto task : data["tasks"]) {
        Task* t = new Task(task["title"]);
        t->SetIsCompleted(task["complete"]);
        tm.AddTask(*t);
    }
}

void FileUtils::WriteTasksFile(string file_path, const TaskManager &tm) {
    string directory = file_path.substr(0, file_path.find_last_of('/'));
    if (directory != file_path && directory != "" && !std::filesystem::exists(directory)) {
        if (!std::filesystem::create_directory(directory)) {
            stringstream err;
            err << "Unable to create directory " << directory << endl;
            throw std::runtime_error(err.str());
        }
    }

    std::ofstream ofs(file_path);
    if (!ofs.is_open()) {
        stringstream err;
        err << "Unable to open file " << file_path << endl;
        throw std::runtime_error(err.str());
    }


    ofs << tm.TasksToJson().dump(4) << endl;

}

