#ifndef TASK_H
#define TASK_H

#include <string>

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

    Task();
    Task(const std::string &taskID, const std::string &clientID, int priority, const std::string &command, const std::string &errorCode);
    static std::string generateId();
};

#endif // TASK_H
