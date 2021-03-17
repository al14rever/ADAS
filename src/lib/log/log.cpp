#include "log.h"
#include <iostream>
#include <fstream>
#include <chrono>
#include <ctime>

void logcln() {
    std::ofstream log;
    log.open("ADAS.log");
    log.close();
}

void log(const char *text) {
    std::time_t time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::tm* now = std::localtime(&time);
    std::ofstream log;
    log.open("ADAS.log", std::fstream::app);
    log << now->tm_mday << '-' << (now->tm_mon + 1) << '-' << (now->tm_year + 1900) << " " << text << "\n";
    log.close();
}

void log(const char *head, const char *error) {
    std::time_t time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::tm* now = std::localtime(&time);
    std::ofstream log;
    log.open("ADAS.log", std::fstream::app);
    log << now->tm_mday << '-' << (now->tm_mon + 1) << '-' << (now->tm_year + 1900) << " " << head << error << "\n";
    log.close();
}