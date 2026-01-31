#include <iostream>
#include <string>
#include <memory>

#include "task_manager.h"

using std::cout, std::endl, std::string, std::unique_ptr;

#define ARGUMENTS "[TASKS_FILE]"

int main(int argv, char* argc[] ) {
    if (argv == 1) {
        cout << argc[0] << " : You must provide at least 1 argument!" << endl;
        return 1;
    }

    string arg1 = argc[1];
    if (arg1 == "-h") {
        cout << argc[0] << " : " << ARGUMENTS << endl;
        return 0;
    }

    unique_ptr<TaskManager> tm(new TaskManager(arg1));
    vector<Task> tasks = tm->GetTasks();
    for (Task t : tasks) {
        string done = t.GetIsCompleted() ? "done" : "not done";
        std::cout << "Task is : " << t.GetTitle() << " - " << done << std::endl;
    }
    return 0;
}
