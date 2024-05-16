#include "PLM.h"
#include <iostream>
#include <thread>

// Fake worker IDs
std::vector<std::string> workerIds2 = {"Worker1", "Worker2", "Worker3"};

void setTimeout(std::function<void()> func, int timeout)
{
    std::thread([func, timeout]()
                {
        std::this_thread::sleep_for(std::chrono::milliseconds(timeout));
        func(); })
        .detach();
}

int main()
{
    PLM &plm = PLM::getInstance();
    plm.createTask("task1", "client1", 1, "command1", 0);
    plm.createTask("task2", "client2", 2, "command2", 0);
    plm.createTask("task3", "client3", 3, "command3", 0);
    plm.createTask("task4", "client1", 4, "command2", 0);
    plm.createTask("task5", "client2", 3, "command3", 0);

    setTimeout([]()
               { PLM::getInstance().updateBasedOnHeartBeats({
                     HeartBeat{"task1", "COMPLETED", 0},
                     HeartBeat{"task2", "COMPLETED", 0},
                 }); },
               2000);
    setTimeout([]()
               { PLM::getInstance().updateBasedOnHeartBeats({
                     HeartBeat{"task3", "FAILED", 1},
                     HeartBeat{"task5", "COMPLETED", 0},
                 }); },
               5000);
    setTimeout([]()
               {
                for(auto workerID: workerIds2)
                    PLM::getInstance().workerFailed(workerID); },
               8000);

    int test;
    std::cin >> test;
    return 0;
}
