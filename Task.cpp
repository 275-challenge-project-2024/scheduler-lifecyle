#include "Task.h"
#include <random>

Task::Task() : priority(0), errorCode(0) {}

Task::Task(const std::string &taskID, const std::string &clientID, int priority, const std::string &command, const std::string &errorCode)
    : taskID(taskID), clientID(clientID), priority(priority), command(command), errorCode(std::stoi(errorCode))
{
    taskStatus = "Initiated";
}

std::string Task::generateId()
{
    const std::string alphanumeric = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, alphanumeric.size() - 1);

    std::string id;
    for (int i = 0; i < 12; ++i)
    {
        id += alphanumeric[dis(gen)];
    }
    return id;
}
