#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <string>

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

#endif // LOGGER_H
