#ifndef _FILE_UTILS__H_
#define _FILE_UTILS__H_

#include <cstdlib>
#include <string>

#include "task_manager.h"

#define HOME_DIR std::string(std::getenv("HOME"))
#define DEFAULT_CONFIG_FILE HOME_DIR + "/.config/ctodo/ctodo.conf"
#define DEFAULT_TASK_FILE "/var/lib/ctodo/tasks.json"
#define DEFAULT_INTERFACE "0.0.0.0"
#define DEFAULT_PORT 8080

class FileUtils {
public:
    static void ParseTasksFile(string file_path, TaskManager &tm);
    static void WriteTasksFile(string file_path, const TaskManager &tm);
    static void ParseConfigFile(string file_path, json &config_data);
    static string GetDefaultTasksFile(const json &config_data);
    static string GetDefaultInterface(const json &config_data);
    static int GetDefaultPort(const json &config_data);
};

#endif //_FILE_UTILS__H_
