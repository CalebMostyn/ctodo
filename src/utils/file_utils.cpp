#include <sstream>
#include <fstream>
#include <filesystem>
#include <string>
#include <stdexcept>
#include <iostream>
#include "json.hpp"
#include "file_utils.h"

using std::stringstream, std::endl, std::string;
using json = nlohmann::json;

void FileUtils::ParseTasksFile(string file_path, TaskManager &tm) {
    
    std::ifstream ifs(file_path);
    if (!ifs.is_open()) {
        // Create file if does not exist
        WriteTasksFile(file_path, tm);
    }

    json data;
    try {
        data = json::parse(ifs);
    } catch (...) {
        stringstream err;
        err << "Unable to parse file " << file_path << endl;
        throw std::runtime_error(err.str());
    }

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

void FileUtils::ParseConfigFile(string file_path, json &config_data) {
    std::ifstream ifs(file_path);
    if (!ifs.is_open()) {
        std::cerr << "Config file does not exist, falling back to server defaults." << endl;
        return;
    }

    try {
        config_data = json::parse(ifs);
    } catch (...) {
        std::cerr << "Invalid config file, falling back to server defaults." << endl;
    }
    return;
}

string FileUtils::GetDefaultTasksFile(const json &config_data) {
    try {
        if (!config_data.is_null()) {
            if (!config_data["server"].is_null()) {
                if (!config_data["server"]["tasks-file"].is_null()) {
                    return (string)config_data["server"]["tasks-file"];
                }
            }
        }
    } catch (...) {
        std::cerr << "Invalid value for tasks file in config, falling back to server default." << endl;
    }
    return DEFAULT_TASK_FILE;
}
string FileUtils::GetDefaultInterface(const json &config_data) {
    try {
        if (!config_data.is_null()) {
            if (!config_data["server"].is_null()) {
                if (!config_data["server"]["interface"].is_null()) {
                    return (string)config_data["server"]["interface"];
                }
            }
        }
    } catch (...) {
        std::cerr << "Invalid value for interface in config, falling back to server default." << endl;
    }
    return DEFAULT_INTERFACE;
}
int FileUtils::GetDefaultPort(const json &config_data) {
    try {
        if (!config_data.is_null()) {
            if (!config_data["server"].is_null()) {
                if (!config_data["server"]["port"].is_null()) {
                    return (int)config_data["server"]["port"];
                }
            }
        }
    } catch (...) {
        std::cerr << "Invalid value for port in config, falling back to server default." << endl;
    }
    return DEFAULT_PORT;
}

bool FileUtils::GetSaveOnRequest(const json &config_data) {
    try {
        if (!config_data.is_null()) {
            if (!config_data["server"].is_null()) {
                if (!config_data["server"]["save-on-request"].is_null()) {
                    return (bool)config_data["server"]["save-on-request"];
                }
            }
        }
    } catch (...) {
        std::cerr << "Invalid value for 'Save on Request' in config, falling back to server default." << endl;
    }
    return DEFAULT_SAVE_ON_REQUEST;
}

