// TaskScheduler.h
#ifndef TASKSCHEDULER_H
#define TASKSCHEDULER_H

#include <functional>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>

struct ScheduledTask {
    std::function<void()> task;
    std::time_t timestamp;

    bool operator>(const ScheduledTask& other) const;
};

class TaskScheduler {
public:
    TaskScheduler();
    ~TaskScheduler();

    void Add(std::function<void()> task, std::time_t timestamp);

private:
    std::priority_queue<ScheduledTask, std::vector<ScheduledTask>, std::greater<ScheduledTask>> tasks;
    std::mutex mtx;
    std::condition_variable cv;
    std::thread worker;
    bool stop;

    void run();
};

#endif
