#ifndef _SAVE_MANAGER__H_
#define _SAVE_MANAGER__H_

#include "task_manager.h"

#include <thread>
#include <mutex>
#include <condition_variable>
#include <memory>

class SaveManager {
private:
    std::shared_ptr<TaskManager> m_task_manager;
    std::thread m_save_thread;
    std::mutex m_save_mutex;
    std::condition_variable m_save_cv;
    bool m_stop_thread = false;

    void SaveTasksAsync();

public:
    SaveManager(std::shared_ptr<TaskManager> tm);
    ~SaveManager();

};

#endif //_SAVE_MANAGER__H_
