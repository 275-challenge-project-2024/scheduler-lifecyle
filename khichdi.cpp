#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <random>
#include <chrono>
#include <thread>
#include <functional>
#include "task_heap.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>
#include <iostream>
#include "SharedMemoryHeap.h"

#define PATHNAME "/tmp" // Make sure this directory exists and is accessible
#define PROJ_ID 123     // An arbitrary number, used to generate a key

// TODO : Remove the capacity from everywhere

std::vector<std::string> workerIds = {"Worker1", "Worker2", "Worker3"};

class Logger
{
public:
    static void info(std::string message)
    {
        std::cout << "INFO : " << message << std::endl;
    }
    static void error(std::string message)
    {
        std::cout << "ERROR : " << message << std::endl;
    }
};

class Task
{
public:
    std::string taskID;
    std::string taskStatus;
    std::string workerID;
    std::string clientID;
    int priority;
    int capacity;
    std::string command;
    int errorCode;

    static std::string generateId()
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
    Task()
    {
        taskID = "";
        taskStatus = "";
        workerID = "";
        clientID = "";
        priority = 0;
        capacity = 0;
        command = "";
        errorCode = 0;
    }

    Task(std::string taskID, std::string clientID, int priority, int capacity, std::string command, std::string errorCode)
    {
        this->taskID = taskID;
        this->taskStatus = "Intiated";
        this->clientID = clientID;
        this->priority = priority;
        this->capacity = capacity;
        this->command = command;
        this->errorCode = 0;
    }
};

void setInterval(std::function<void()> func, int interval)
{
    std::thread([func, interval]()
                {
        while (true) {
            func();
            std::this_thread::sleep_for(std::chrono::milliseconds(interval));
        } })
        .detach();
}

void setTimeout(std::function<void()> func, int timeout)
{
    std::thread([func, timeout]()
                {
        std::this_thread::sleep_for(std::chrono::milliseconds(timeout));
        func(); })
        .detach();
}

class Scheduler
{
public:
    // FAKE IMPLEMENTATION
    static std::string getWorkerId(Task task)
    {
        // Implement your logic here
        // get random worker from workerIds
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, workerIds.size() - 1);
        return workerIds[dis(gen)];
    }
};

class Registry
{
public:
    Registry() {}
    static Registry getInstance()
    {
        static Registry instance;
        return instance;
    }
    void sentTask(Task task)
    {
        // send the task to the worker
    }
};

class Stub
{
public:
    void sendTask(Task task)
    {
        Logger::info("Task sent to worker with taskID : " + task.taskID + " and clientID : " + task.clientID + " and workerID : " + task.workerID + " and command : " + task.command + " and errorCode : " + std::to_string(task.errorCode) + " and priority : " + std::to_string(task.priority));
        // Registry::getInstance().sentTask(task);
    }
    void sendTaskCompleted(Task task)
    {
        Logger::info("Task completed with taskID : " + task.taskID + " and clientID : " + task.clientID + " and workerID : " + task.workerID + " and command : " + task.command + " and errorCode : " + std::to_string(task.errorCode) + " and priority : " + std::to_string(task.priority));
        // Registry::getInstance().sentTask(task);
    }
};

class HeartBeat
{
public:
    std::string taskID;
    std::string status;
    int errorCode;
};

// class SharedMemoryHeap
// {
// public:
//     key_t SHM_KEY;
//     int shm_id;
//     HeapData *shm_data;
//     SharedMemoryHeap()
//     {
//         // Generate a unique key from a pathname and a project identifier
//         SHM_KEY = ftok(PATHNAME, PROJ_ID);
//         if (SHM_KEY == -1)
//         {
//             perror("ftok failed");
//             exit(1);
//         }
//         shm_id = shmget(SHM_KEY, sizeof(HeapData), IPC_CREAT | 0666);
//         if (shm_id == -1)
//         {
//             perror("shmget failed");
//             exit(1);
//         }
//         shm_data = (HeapData *)shmat(shm_id, NULL, 0);
//         if (shm_data == (HeapData *)-1)
//         {
//             perror("shmat failed");
//             exit(1);
//         }
//     }
//     ~SharedMemoryHeap()
//     {
//         if (shmdt(shm_data) == -1)
//         {
//             perror("shmdt failed");
//             exit(1);
//         }
//         if (shmctl(shm_id, IPC_RMID, NULL) == -1)
//         {
//             perror("shmctl(IPC_RMID) failed");
//             exit(1);
//         }
//     }
//     void pushToSharedMemory(Task task)
//     {

//         auto now = std::chrono::system_clock::now();
//         auto duration = now.time_since_epoch();
//         auto timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();

//         // convert timestamp to string
//         std::string timestamp_str = std::to_string(timestamp);

//         heap_push(shm_data, task.taskID.c_str(), timestamp_str.c_str(), task.priority);
//     }
// };

class PLM
{
private:
    std::unordered_map<std::string, Task> storage; // Map to store taskId to Taskinfo

    // Private constructor to prevent instantiation
    PLM()
    {
        key_t SHM_KEY;
        // Generate a unique key from a pathname and a project identifier
        SHM_KEY = ftok(PATHNAME, PROJ_ID);
        if (SHM_KEY == -1)
        {
            perror("ftok failed");
            exit(1);
        }
        std::cout << "HERE" << std::endl;
        int shm_id = shmget(SHM_KEY, sizeof(HeapData), IPC_CREAT | 0666);
        if (shm_id >= 0)
        {
            // If shared memory segment exists, delete it
            if (shmctl(shm_id, IPC_RMID, NULL) == -1)
            {
                perror("shmctl(IPC_RMID) failed");
                exit(1);
            }
        }
        // create one function that periodically checks the status of the task
        // if the task is not completed then reassign the task to the worker
        // setInterval([]()
        //             { std::cout << "Checking the status of the task\n"; },
        //             1000);
    }

public:
    // Get instance function to return the singleton instance
    static PLM &getInstance()
    {
        static PLM instance;
        return instance;
    }

    // TODO : if scheduler is not responsing then at least store the task and periodically try to reassign the task using scheduler
    // TODO : (TIMEOUT SCENARIO) periodically check the status of the task and if it is not completed then reassign the task to the worker

    void pushToSharedMemory(Task task)
    {
        // TODO 1.1) Create New Task : some heavy logic to push the task to the shared memory
        // std::string workerId = Scheduler::getWorkerId(task);
        // task.workerID = workerId;
        // storage[task.taskID] = task;
        SharedMemoryHeap &shmHeap = SharedMemoryHeap::getInstance();
        shmHeap.pushToSharedMemory(task.taskID, task.priority);
    }

    void createTask(std::string taskID, std::string clientID, int priority, int capacity, std::string command, std::string errorCode)
    {
        Task newTask(taskID, clientID, priority, capacity, command, errorCode);

        // add to local variable
        storage[newTask.taskID] = newTask;

        // add to shared priority queue
        pushToSharedMemory(newTask);
    }

    // client => task(priority) => PLM => local storage => shared memory =>
    // scheduler => pop from shared memory => find related worker => PLM.assignWorker(taskID,workerID)

    void assignWorker(std::string taskID, std::string workerID)
    {
        Task task = storage[taskID];

        // TODO : check if the task is there in the storage
        task.workerID = workerID;
        task.taskStatus = "ASSIGNED";

        // TODO : add mutex here (wherever storage is updated in the code)
        storage[task.taskID] = task;

        // once the assignment is done we need to send that to worker
        sendTask(task);
    }

    Stub getRelatedStub(std::string workerID)
    {
        // get the related stub
        // Mr. D will provide the implementation
        return Stub();
    }

    void sendTask(Task task)
    {
        Stub stub = getRelatedStub(task.workerID);

        // send this task to the worker
        stub.sendTask(task);
    }

    void updateBasedOnHeartBeats(std::vector<HeartBeat> heartbeats)
    {
        for (HeartBeat hb : heartbeats)
        {
            if (storage.find(hb.taskID) != storage.end())
            {
                Task task = storage[hb.taskID];
                task.taskStatus = hb.status;

                // assuming they will send 0 for success scenario
                task.errorCode = hb.errorCode ? hb.errorCode : 0;
                storage[hb.taskID] = task;
                if (hb.errorCode != 0)
                {
                    // some error occured
                    handleTaskError(task);
                }
                else if (hb.status == "COMPLETED")
                {
                    // task is completed
                    // remove from the storage
                    // delete that taskID from the storage
                    storage.erase(hb.taskID);
                    updateTaskStatusToCompleted(task);
                }
            }
        }
    }

    void updateTaskStatusToCompleted(Task task)
    {
        // remove from the storage
        // TODO : FILL IN THE DETAILS
        Stub stub = getRelatedStub(task.clientID);
        stub.sendTaskCompleted(task);
    }

    void workerFailed(std::string workerId)
    {
        Logger::error("Worker with workerId : " + workerId + " failed");
        for (auto it = storage.begin(); it != storage.end(); it++)
        {
            if (it->second.workerID == workerId)
            {
                if (it->second.taskStatus != "COMPLETED")
                    handleTaskError(it->second);
            }
        }
    }

    void handleTaskError(Task task)
    {
        Logger::error("Task with taskID : " + task.taskID + " failed with errorCode : " + std::to_string(task.errorCode));
        // TODO : add logs
        // when something goes wrong get new workerId for the task
        task.workerID = "";
        task.taskStatus = "REASSIGNING";
        task.priority += 1;
        storage[task.taskID] = task;

        // add to shared priority queue
        pushToSharedMemory(task);
    }
};

int main()
{

    // Example usage

    // FAKE IMPLEMENTATION
    PLM &plm = PLM::getInstance();
    SharedMemoryHeap::getInstance();

    PLM::getInstance().createTask("task1", "client1", 1, 1, "command1", "0");
    PLM::getInstance().createTask("task2", "client2", 2, 1, "command2", "0");
    PLM::getInstance().createTask("task3", "client3", 3, 1, "command3", "0");
    PLM::getInstance().createTask("task4", "client1", 4, 1, "command2", "0");
    PLM::getInstance().createTask("task5", "client2", 3, 1, "command3", "0");

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
                for(auto workerID: workerIds)
                    PLM::getInstance().workerFailed(workerID); },
               8000);

    int test;
    std::cin >> test;
    return 0;
}
