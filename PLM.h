#ifndef PLM_H
#define PLM_H

#include "Logger.h"
#include "Task.h"
#include <unordered_map>
#include <vector>
#include <mutex>
#include <functional>
#include <thread>
#include <chrono>

class Stub
{
public:
    void sendTask(Task task)
    {
        Logger::info("Task sent to worker with taskID : " + task.taskID + " and clientID : " + task.clientID + " and workerID : " + task.workerID + " and command : " + task.command + " and errorCode : " + std::to_string(task.errorCode) + " and priority : " + std::to_string(task.priority));
    }
    void sendTaskCompleted(Task task)
    {
        Logger::info("Task completed with taskID : " + task.taskID + " and clientID : " + task.clientID + " and workerID : " + task.workerID + " and command : " + task.command + " and errorCode : " + std::to_string(task.errorCode) + " and priority : " + std::to_string(task.priority));
    }
};

class HeartBeat
{
public:
    std::string taskID;
    std::string status;
    int errorCode;
};

class PLM
{
private:
    std::unordered_map<std::string, Task> storage;
    std::mutex storageMutex;

    // Private constructor to prevent instantiation
    PLM();

public:
    static PLM &getInstance();

    void pushToSharedMemory(Task task);
    void createTask(std::string taskID, std::string clientID, int priority, std::string command, int errorCode);
    void assignWorker(std::string taskID, std::string workerID);
    Stub getRelatedStub(std::string workerID);
    void sendTask(Task task);
    void updateBasedOnHeartBeats(std::vector<HeartBeat> heartbeats);
    void updateTaskStatusToCompleted(Task task);
    void workerFailed(std::string workerId);
    void handleTaskError(Task task);
};

#endif // PLM_H
