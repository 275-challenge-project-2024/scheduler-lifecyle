#include "Stub.h"
#include "Logger.h"

void Stub::sendTask(Task &task)
{
    // Log the action
    Logger::info("Sending task to worker: Task ID = " + task.taskID + ", Worker ID = " + task.workerID + ", Command = " + task.command);
    // Simulate sending the task to the worker. Actual implementation would involve communication mechanisms such as sockets, message queues, etc.
}

void Stub::sendTaskCompleted(Task &task)
{
    // Log the action
    Logger::info("Task completed: Task ID = " + task.taskID + ", Worker ID = " + task.workerID + ", Status = " + task.taskStatus);
    // Here, you could handle follow-up actions post task completion, such as updating databases, notifying clients, etc.
}
