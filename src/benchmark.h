#ifndef BENCHMARK_H
#define BENCHMARK_H

#include <chrono>
#include <string>
#include <iostream>

namespace Cascade
{

static std::chrono::steady_clock::time_point timerBegin;

void startTimer()
{
    timerBegin = std::chrono::steady_clock::now();
}

void stopTimerAndPrint(const std::string& s)
{
    static std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

    std::cout << s + " " << std::chrono::duration_cast<std::chrono::milliseconds>(end - timerBegin).count() << "[ms]" << std::endl;
}

}

#endif // BENCHMARK_H
