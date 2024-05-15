#ifndef TASK_H
#define TASK_H

#include <string>
#include <random>

class Task
{
public:
    std::string taskID;
    std::string taskStatus;
    std::string workerID;
    std::string clientID;
    int priority;
    std::string command;
    int errorCode;

    static std::string generateId();

    Task();
    Task(std::string taskID, std::string clientID, int priority, std::string command, int errorCode);
};

#endif // TASK_H
