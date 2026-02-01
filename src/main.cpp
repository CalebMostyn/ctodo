#include <iostream>
#include <string>
#include "httplib.h"

#include "task_manager.h"

using std::cout, std::endl, std::string, std::unique_ptr;

#define ARGUMENTS "[TASKS_FILE]"

std::atomic<bool> stop_flag = false;
void signal_handler(int signum) {
    cout << endl << "Signal (" << signum << ") received, shutting down..." << endl;
    stop_flag = true;
}

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

    TaskManager tm(arg1);
    vector<Task> tasks = tm.GetTasks();
    for (Task t : tasks) {
        string done = t.GetIsCompleted() ? "done" : "not done";
        std::cout << "Task is : " << t.GetTitle() << " - " << done << std::endl;
    }

    httplib::Server svr;
    svr.Get("/hi", [](const httplib::Request &, httplib::Response &res) {
      res.set_content("Hello World!", "text/plain");
    });

    std::signal(SIGINT, signal_handler);
    std::signal(SIGTERM, signal_handler);

    // start http server on seperate thread
    std::thread server_thread([&]() {
        cout << "Server listening on port 8080..." << endl;
        svr.listen("0.0.0.0", 8080);
    });

    // poll for killed server
    while (!stop_flag) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    // stop server and join its thread
    svr.stop();
    server_thread.join();
    cout << "Server stopped." << endl;
    
    return 0;
}
