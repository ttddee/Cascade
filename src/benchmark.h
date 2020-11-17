#ifndef BENCHMARK_H
#define BENCHMARK_H

#include <chrono>
#include <string>
#include <iostream>

namespace Cascade
{

static std::chrono::steady_clock::time_point timerBegin;
static std::chrono::steady_clock::time_point timerEnd;

void startTimer()
{
    timerBegin = std::chrono::steady_clock::now();
}

void stopTimerAndPrint(const std::string& s)
{
    timerEnd = std::chrono::steady_clock::now();

    std::cout << s + " " << std::chrono::duration_cast<std::chrono::milliseconds>(timerEnd - timerBegin).count() << "[milliseconds]" << std::endl;
    //std::cout << s + " " << std::chrono::duration_cast<std::chrono::microseconds>(timerEnd - timerBegin).count() << "[microseconds]" << std::endl;
}

}

#endif // BENCHMARK_H
