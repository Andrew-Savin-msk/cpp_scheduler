#include "TaskScheduler.h"
#include <iostream>

bool ScheduledTask::operator>(const ScheduledTask& other) const {
    return timestamp > other.timestamp;
}

TaskScheduler::TaskScheduler() : stop(false) {
    worker = std::thread(&TaskScheduler::run, this);
}

TaskScheduler::~TaskScheduler() {
    {
        std::unique_lock<std::mutex> lock(mtx);
        stop = true;
        cv.notify_all();
    }
    if (worker.joinable()) {
        worker.join();
    }
}

void TaskScheduler::Add(std::function<void()> task, std::time_t timestamp) {
    std::unique_lock<std::mutex> lock(mtx);
    bool notify = tasks.empty() || timestamp < tasks.top().timestamp;
    tasks.push({task, timestamp});
    if (notify) {
        cv.notify_one();
    }
}

void TaskScheduler::run() {
    std::unique_lock<std::mutex> lock(mtx);
    while (true) {
        if (stop && tasks.empty()) {
            break;
        }

        if (tasks.empty()) {
            cv.wait(lock, [this] { return stop || !tasks.empty(); });
            continue;
        }

        auto nextTask = tasks.top();
        auto now = std::chrono::system_clock::now();
        auto taskTime = std::chrono::system_clock::from_time_t(nextTask.timestamp);

        if (taskTime <= now) {
            tasks.pop();
            lock.unlock();
            nextTask.task();
            lock.lock();
        } else {
            cv.wait_until(lock, taskTime);
        }
    }
}
