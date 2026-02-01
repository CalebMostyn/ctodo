#ifndef _FILE_UTILS__H_
#define _FILE_UTILS__H_

#include <string>
#include "task_manager.h"

#define TASKS_HEADER "[TASKS]"

class FileUtils {
public:
    static void ParseTasksFile(string file_path, TaskManager &tm);
    static void WriteTasksFile(string file_path, const TaskManager &tm);
};

#endif //_FILE_UTILS__H_
