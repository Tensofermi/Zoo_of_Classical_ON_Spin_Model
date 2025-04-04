#pragma once
#include <bits/stdc++.h>
#include "../Utils/Utils.hpp"
#include "../Clock/Clock.hpp"
#include "../IOControl/IOControl.hpp"
#include "../../config/Parameter_config.hpp"
#define sprintf_s sprintf

class HistogramBase
{
    Clock& ck;
    IOControl& io;

    std::string Des;
    int disType;    // 0: linear, 1: log
    unsigned long disCount, disNum, disMax, disMin;
    double a, b;    // range [a,b]
    std::vector<long double> disData; 
    bool saveOrNot = true;     // check if save or not

public:
    HistogramBase(std::string _Type, std::string _Des, double _a, double _b, unsigned long _size, Clock& _ck, IOControl& _io);
    void disAdd(double _obs);
    void disDel(double _obs);
    void disPush(unsigned long _index, double _obs);
    void disPull(unsigned long _index, double _obs);
    void saveDis();

    unsigned long obs2Bin(double _obs);
    double bin2Obs(unsigned long _bin);

    // logbin
    std::vector<int> mappingArray;
    int logNum = 2000;
    int interval = 1;
    int modsteps = 64;

};