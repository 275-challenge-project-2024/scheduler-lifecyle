#include "Task.h"

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

Task::Task()
    : taskID(""), taskStatus(""), workerID(""), clientID(""), priority(0), command(""), errorCode(0)
{
}

Task::Task(std::string taskID, std::string clientID, int priority, std::string command, int errorCode)
    : taskID(taskID), taskStatus("Initiated"), workerID(""), clientID(clientID), priority(priority), command(command), errorCode(errorCode)
{
}
