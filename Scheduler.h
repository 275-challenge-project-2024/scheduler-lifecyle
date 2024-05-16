#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "Task.h"
#include <vector>

class Scheduler
{
public:
    static std::string getWorkerId(const Task &task);
};

#endif // SCHEDULER_H
