#include <iostream>
#include <functional>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>

struct ScheduledTask {
    std::function<void()> task;
    std::time_t timestamp;

    bool operator>(const ScheduledTask& other) const {
        return timestamp > other.timestamp;
    }
};

class TaskScheduler {
public:
    TaskScheduler() : stop(false) {
        worker = std::thread(&TaskScheduler::run, this);
    }

    ~TaskScheduler() {
        {
            std::unique_lock<std::mutex> lock(mtx);
            stop = true;
            cv.notify_all();
        }
        if (worker.joinable()) {
            worker.join();
        }
    }

    void Add(std::function<void()> task, std::time_t timestamp) {
        std::unique_lock<std::mutex> lock(mtx);
        bool notify = tasks.empty() || timestamp < tasks.top().timestamp;
        tasks.push({task, timestamp});
        if (notify) {
            cv.notify_one();
        }
    }

private:
    std::priority_queue<ScheduledTask, std::vector<ScheduledTask>, std::greater<ScheduledTask>> tasks;
    std::mutex mtx;
    std::condition_variable cv;
    std::thread worker;
    bool stop;

    void run() {
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
};

int main() {
    TaskScheduler scheduler;

    auto now = std::time(nullptr);

    scheduler.Add([]() { std::cout << "Task 1 executed\n"; }, now + 2);
    scheduler.Add([]() { std::cout << "Task 2 executed\n"; }, now + 1);
    scheduler.Add([]() { std::cout << "Task 3 executed\n"; }, now + 3);

    std::this_thread::sleep_for(std::chrono::seconds(5));

    return 0;
}
