#include <iostream>
#include <ctime>
#include "TaskScheduler.h"

int main() {
    TaskScheduler scheduler;

    auto now = std::time(nullptr);

    scheduler.Add([]() { std::cout << "Task 1 executed\n"; }, now + 2);
    scheduler.Add([]() { std::cout << "Task 2 executed\n"; }, now + 1);
    scheduler.Add([]() { std::cout << "Task 3 executed\n"; }, now + 3);

    // Allow some time for tasks to execute
    std::this_thread::sleep_for(std::chrono::seconds(5));

    return 0;
}
