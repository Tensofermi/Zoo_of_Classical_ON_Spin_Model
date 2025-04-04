#pragma once
#include <bits/stdc++.h>
#include "../Utils/Utils.hpp"

class Clock
{
    time_t Now;
    time_t Last;
    time_t Origin;
    
    std::string printTime(time_t time);               // print time point
    std::string printDT(time_t time1, time_t time2);  // print time interval

public:
    Clock();
    void reset();                                     // resets to the current time
    std::string exportNow();                          // outputs the current time
    std::string exportTime();                         // outputs the time interval since the last reset
    std::string exportTime_sec();                     // outputs the time interval since the last reset in seconds
    std::string formTime(time_t time);                // formats seconds into a standard time format
    std::string tick();                               // outputs the time interval since the last tick
    
};
