#ifndef HEARTBEAT_H
#define HEARTBEAT_H

#include <string>

class HeartBeat
{
public:
    std::string taskID;
    std::string status;
    int errorCode;

    HeartBeat(const std::string &taskID, const std::string &status, int errorCode)
        : taskID(taskID), status(status), errorCode(errorCode) {}
};

#endif // HEARTBEAT_H
