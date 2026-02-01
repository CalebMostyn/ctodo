#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include "httplib.h"

#include "task_manager.h"
#include "request_manager.h"

using std::cout, std::cerr, std::endl, std::string, std::shared_ptr, std::vector, std::atomic, std::map;
using namespace httplib;

const vector<string> ARGUMENTS = {"-f", "-i", "-p"};
const map<string, string> ARGUMENTS_HELP = {{"-f", "Tasks file to use. Created if does not exist. Defaults to /var/lib/ctodo/tasks.txt."},
                                        {"-i", "Interface to bind server to. Defaults to '0.0.0.0'."},
                                        {"-p", "Port to bind server to. Defaults to 8080."}};

#define ALL_ARGUMENTS_HELP "[-f TASKS_FILE] [-i INTERFACE_ADDR] [-p PORT]"
#define DEFAULT_FILE "/var/lib/ctodo/tasks.txt"
#define DEFAULT_INTERFACE "0.0.0.0"
#define DEFAULT_PORT 8080


atomic<bool> stop_flag = false;
void signal_handler(int signum) {
    cout << endl << "Signal (" << signum << ") received, shutting down..." << endl;
    stop_flag = true;
}

int main(int argv, char* argc[] ) {
    string tasks_file = DEFAULT_FILE;
    string interface = DEFAULT_INTERFACE;
    int port = DEFAULT_PORT;
    if (argv > 1) {
        if ((string)argc[1] == "-h") {
            if (argv == 2) {
                cout << argc[0] << " : " << ALL_ARGUMENTS_HELP << endl;
            } else if (ARGUMENTS_HELP.contains((string)argc[2])) {
                cout << argc[0] << " " << argc[2] << " : " << ARGUMENTS_HELP.at((string)argc[2]) << endl;
            } else {
                cout << argc[0] << " : Unrecognized argument " << argc[2] << endl;
            }
            return 0;
        } 
        for (int ii = 1; ii < argv; ii++) {
            if ((string)argc[ii] == "-f") {
                if (ii == argv - 1) {
                    cerr << argc[0] << " : Missing argument TASKS_FILE" << endl;
                    return 1;
                }
                tasks_file = argc[ii + 1];
                ii++;
            } else if ((string)argc[ii] == "-i") {
                if (ii == argv - 1) {
                    cerr << argc[0] << " : Missing argument INTERFACE_ADDR" << endl;
                    return 1;
                }
                interface = argc[ii + 1];
                ii++;
            } else if ((string)argc[ii] == "-p") {
                if (ii == argv - 1) {
                    cerr << argc[0] << " : Missing argument PORT" << endl;
                    return 1;
                }
                try {
                    port = std::stoi(argc[ii + 1]);
                } catch (...) {
                    cerr << argc[0] << " : Invalid argument PORT" << argc[ii + 1] << endl;
                    return 1;
                }
                ii++;
            } else {
                cerr << argc[0] << " : Unrecognized argument " << argc[ii] << endl;
                return 1;
            }
        }
    }


    // Setup TaskManager
    shared_ptr<TaskManager> tm;
    try {
        tm = std::make_shared<TaskManager>(tasks_file);
    } catch (const std::exception& e) {
        // failed to initialize
        cerr << e.what();
        return 1;
    }

    // Setup RequestManager
    RequestManager rm(tm);

    // Setup Server
    Server svr;
    rm.SetupServer(svr);

    // Signal catching
    std::signal(SIGINT, signal_handler);
    std::signal(SIGTERM, signal_handler);

    std::thread server_thread;
    try {
        // start http server on seperate thread
        server_thread = std::thread([&]() {
            cout << "Server listening on " << interface << ":" << port << "..." << endl;
            svr.listen(interface, port);
        });
    } catch (...) {
        cerr << argc[0] << " : Failed to open server on " << interface << ":" << port << endl;
        return 1;
    }

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
