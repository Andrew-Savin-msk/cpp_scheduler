// TaskScheduler.cpp
#include "TaskScheduler.h"
#include <iostream>

// Implement the comparison operator for ScheduledTask
bool ScheduledTask::operator>(const ScheduledTask& other) const {
    return timestamp > other.timestamp;
}

// TaskScheduler Constructor
TaskScheduler::TaskScheduler() : stop(false) {
    // Start the worker thread
    worker = std::thread(&TaskScheduler::run, this);
}

// TaskScheduler Destructor
TaskScheduler::~TaskScheduler() {
    {
        // Signal the worker thread to stop
        std::unique_lock<std::mutex> lock(mtx);
        stop = true;
        cv.notify_all();
    }
    // Join the worker thread
    if (worker.joinable()) {
        worker.join();
    }
}

// Add Method
void TaskScheduler::Add(std::function<void()> task, std::time_t timestamp) {
    std::unique_lock<std::mutex> lock(mtx);
    bool notify = tasks.empty() || timestamp < tasks.top().timestamp;
    tasks.push({task, timestamp});
    // Notify the worker thread if necessary
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
            // Execute the task
            tasks.pop();
            lock.unlock();
            nextTask.task();
            lock.lock();
        } else {
            // Wait until the next task's scheduled time or a new task is added
            cv.wait_until(lock, taskTime);
        }
    }
}
