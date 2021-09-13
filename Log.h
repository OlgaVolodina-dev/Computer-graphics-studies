#ifndef LOG_H
#define LOG_H

#include <iostream>
#include "fmt/core.h"

class Log {
public:
    template<typename ... Args>
    static void Info( const std::string& format, Args ... args )
    {
        std::cout << "[INFO] " << fmt::format(format, args ...) << std::endl; 
    }

    template<typename ... Args>
    static void Error( const std::string& format, Args ... args )
    {
        std::cout << "[ERROR] " << fmt::format(format, args ...) << std::endl; 
    }

    template<typename ... Args>
    static void Debug( const std::string& format, Args ... args )
    {
        std::cout << "[DEBUG] " << fmt::format(format, args ...)<< std::endl; 
    }
};

#endif // LOG_H