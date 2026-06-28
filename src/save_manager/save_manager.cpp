#include "save_manager.h"
#include "settings.h"

#include <iostream>
#include <chrono>

SaveManager::SaveManager(std::shared_ptr<TaskManager> tm) {
    m_task_manager = tm;
    m_save_thread = std::thread(&SaveManager::SaveTasksAsync, this);
}

SaveManager::~SaveManager() {
    // notify save thread to stop
    m_save_mutex.lock();
    m_stop_thread = true;
    m_save_mutex.unlock();

    // wake up thread if sleeping
    m_save_cv.notify_one();
    // join worker thread
    m_save_thread.join();
}

void SaveManager::SaveTasksAsync() {
    std::unique_lock lock(m_save_mutex); // wrap scope in mutex lock/unlock
    while (!m_stop_thread) {
        auto sleep_time = std::chrono::duration<double>(Settings::Instance()->m_save_period);
        // mutex is released while sleeping so it can be awoken
        m_save_cv.wait_for(lock, sleep_time, [this] { return m_stop_thread; });

        // if thread was requested to stop, break out of loop and join worker thread
        if (m_stop_thread) {
            break;
        }

        // unlock mutex so that file save doesn't halt anything waiting on mutex
        lock.unlock();
        // save to file
        std::cout << "saving to file!!!!" << std::endl;
        m_task_manager->Save();
        lock.lock();
    }
}
