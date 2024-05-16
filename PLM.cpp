#include "PLM.h"
#include "Scheduler.h"
#include "Logger.h"

PLM::PLM()
{
    // The constructor is private, used only for the singleton instance
}

PLM &PLM::getInstance()
{
    static PLM instance;
    return instance;
}

void PLM::createTask(const std::string &taskID, const std::string &clientID, int priority, const std::string &command, const std::string &errorCode)
{
    Task newTask(taskID, clientID, priority, command, errorCode);
    storage[newTask.taskID] = newTask;
    pushToSharedMemory(newTask);
}

void PLM::pushToSharedMemory(Task &task)
{
    task.workerID = Scheduler::getWorkerId(task);
    task.taskStatus = "ASSIGNED";
    storage[task.taskID] = task;
    sendTask(task);
}

void PLM::assignWorker(const std::string &taskID, const std::string &workerID)
{
    if (storage.find(taskID) != storage.end())
    {
        Task &task = storage[taskID];
        task.workerID = workerID;
        task.taskStatus = "ASSIGNED";
        sendTask(task);
    }
}

void PLM::updateBasedOnHeartBeats(const std::vector<HeartBeat> &heartbeats)
{
    for (const HeartBeat &hb : heartbeats)
    {
        if (storage.find(hb.taskID) != storage.end())
        {
            Task &task = storage[hb.taskID];
            task.taskStatus = hb.status;
            task.errorCode = hb.errorCode;
            if (hb.errorCode != 0)
            {
                handleTaskError(task);
            }
            else if (hb.status == "COMPLETED")
            {
                storage.erase(hb.taskID);
                updateTaskStatusToCompleted(task);
            }
        }
    }
}

void PLM::workerFailed(const std::string &workerId)
{
    Logger::error("Worker with workerId: " + workerId + " failed");
    for (auto &it : storage)
    {
        if (it.second.workerID == workerId && it.second.taskStatus != "COMPLETED")
        {
            handleTaskError(it.second);
        }
    }
}

void PLM::handleTaskError(Task &task)
{
    Logger::error("Task with taskID: " + task.taskID + " failed with errorCode: " + std::to_string(task.errorCode));
    task.workerID = "";
    task.taskStatus = "REASSIGNING";
    task.priority++;
    pushToSharedMemory(task);
}

Stub PLM::getRelatedStub(const std::string &workerID)
{
    // Implementation for getting stub based on workerID. This could depend on network or other parameters.
    return Stub();
}

void PLM::sendTask(Task &task)
{
    Stub stub = getRelatedStub(task.workerID);
    stub.sendTask(task);
}

void PLM::updateTaskStatusToCompleted(Task &task)
{
    Stub stub = getRelatedStub(task.clientID);
    stub.sendTaskCompleted(task);
}
