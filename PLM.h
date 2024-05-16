#ifndef PLM_H
#define PLM_H

#include "Task.h"
#include "Stub.h"
#include "HeartBeat.h"
#include <unordered_map>
#include <vector>
#include <string>

class PLM
{
private:
    std::unordered_map<std::string, Task> storage; // Map to store taskId to Task info
    PLM();                                         // Private constructor for the singleton

public:
    static PLM &getInstance();

    void createTask(const std::string &taskID, const std::string &clientID, int priority, const std::string &command, const std::string &errorCode);
    void pushToSharedMemory(Task &task);
    void assignWorker(const std::string &taskID, const std::string &workerID);
    void updateBasedOnHeartBeats(const std::vector<HeartBeat> &heartbeats);
    void workerFailed(const std::string &workerId);
    void handleTaskError(Task &task);

    void sendTask(Task &task);
    void updateTaskStatusToCompleted(Task &task);
    Stub getRelatedStub(const std::string &workerID);
};

#endif // PLM_H
