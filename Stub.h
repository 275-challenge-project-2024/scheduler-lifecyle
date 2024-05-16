#ifndef STUB_H
#define STUB_H

#include "Task.h"

class Stub
{
public:
    void sendTask(Task &task);
    void sendTaskCompleted(Task &task);
};

#endif // STUB_H
