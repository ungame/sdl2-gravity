#include <chrono>
#include <ctime>
#include <string>

#include "Time.hpp"

Time::Time()
{
    _time = Now();
}

double Time::Since(Time* t)
{
    std::chrono::duration<double> elapsed = Now() - t->Started();

    return elapsed.count();
}

std::string Time::String()
{
    std::time_t tt = std::chrono::system_clock::to_time_t(_time);
    auto lt = localtime(&tt);
    char strt[] = {0};
    strftime(strt, TIME_FORMAT_LENGTH, "%Y-%m-%d %H:%M:%S", lt);
    return std::string(strt);
}
