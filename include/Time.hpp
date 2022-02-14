#ifndef TIME_HPP
#define TIME_HPP

#include <chrono>
#include <ctime>
#include <string>

#define TIME_FORMAT_LENGTH 20

class Time
{
    public:
        Time();
        inline std::chrono::time_point<std::chrono::system_clock> Started() { return _time; } 
        std::string String();

    public:
        static double Since(Time* t);
        inline static std::chrono::time_point<std::chrono::system_clock> Now() { return std::chrono::system_clock::now(); }
        

    private:
        std::chrono::time_point<std::chrono::system_clock> _time;
};

#endif
