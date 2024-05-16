#include "Scheduler.h"
#include <random>

extern std::vector<std::string> workerIds2;

std::string Scheduler::getWorkerId(const Task &task)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, workerIds2.size() - 1);
    return workerIds2[dis(gen)];
}
