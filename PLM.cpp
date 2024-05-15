#include "PLM.h"
#include "Logger.h"
#include <iostream>
#include <random>

// Fake worker IDs
std::vector<std::string> workerIds = {"Worker1", "Worker2", "Worker3"};

PLM::PLM()
{
    // Periodically check the status of the task
    std::thread([this]()
                {
                    while (true) {
                        std::this_thread::sleep_for(std::chrono::milliseconds(10000));
                        std::lock_guard<std::mutex> lock(storageMutex);
                        std::cout << "Checking the status of the task\n"; 
                        for (auto& pair : storage) {
                            Task& task = pair.second;
                            if (task.taskStatus != "COMPLETED" && task.taskStatus != "REASSIGNING") {
                                std::cout << "Reassigning task " << task.taskID << "\n";
                                handleTaskError(task);
                            }
                        }
                    } })
        .detach();
}

PLM &PLM::getInstance()
{
    static PLM instance;
    return instance;
}

void PLM::pushToSharedMemory(Task task)
{
    std::lock_guard<std::mutex> lock(storageMutex);
    std::string workerId = workerIds[std::rand() % workerIds.size()];
    task.workerID = workerId;
    storage[task.taskID] = task;
}

void PLM::createTask(std::string taskID, std::string clientID, int priority, std::string command, int errorCode)
{
    Task newTask(taskID, clientID, priority, command, errorCode);

    {
        std::lock_guard<std::mutex> lock(storageMutex);
        storage[newTask.taskID] = newTask;
    }

    pushToSharedMemory(newTask);
}

void PLM::assignWorker(std::string taskID, std::string workerID)
{
    std::lock_guard<std::mutex> lock(storageMutex);

    auto it = storage.find(taskID);
    if (it != storage.end())
    {
        Task &task = it->second;
        task.workerID = workerID;
        task.taskStatus = "ASSIGNED";
        storage[task.taskID] = task;

        sendTask(task);
    }
    else
    {
        std::cerr << "Task " << taskID << " not found in storage." << std::endl;
    }
}

Stub PLM::getRelatedStub(std::string workerID)
{
    return Stub();
}

void PLM::sendTask(Task task)
{
    Stub stub = getRelatedStub(task.workerID);
    stub.sendTask(task);
}

void PLM::updateBasedOnHeartBeats(std::vector<HeartBeat> heartbeats)
{
    std::lock_guard<std::mutex> lock(storageMutex);

    for (HeartBeat hb : heartbeats)
    {
        auto it = storage.find(hb.taskID);
        if (it != storage.end())
        {
            Task &task = it->second;
            task.taskStatus = hb.status;
            task.errorCode = hb.errorCode ? hb.errorCode : 0;
            storage[hb.taskID] = task;

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

void PLM::updateTaskStatusToCompleted(Task task)
{
    Stub stub = getRelatedStub(task.clientID);
    stub.sendTaskCompleted(task);
}

void PLM::workerFailed(std::string workerId)
{
    Logger::error("Worker with workerId : " + workerId + " failed");
    for (auto it = storage.begin(); it != storage.end(); ++it)
    {
        if (it->second.workerID == workerId)
        {
            if (it->second.taskStatus != "COMPLETED")
                handleTaskError(it->second);
        }
    }
}

void PLM::handleTaskError(Task task)
{
    Logger::error("Task with taskID : " + task.taskID + " failed with errorCode : " + std::to_string(task.errorCode));
    task.workerID = "";
    task.taskStatus = "REASSIGNING";
    task.priority += 1;
    storage[task.taskID] = task;

    pushToSharedMemory(task);
}
