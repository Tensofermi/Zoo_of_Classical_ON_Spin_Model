#pragma once
#include <bits/stdc++.h>
#include "../Utils/Utils.hpp"
#include "../Clock/Clock.hpp"
#include "../IOControl/IOControl.hpp"
#include "../../config/Parameter_config.hpp"
#include "HistogramBase.hpp"
#define sprintf_s sprintf

class Histogram
{
    Clock& ck;
    IOControl& io;
    Parameter& para;

    HistogramBase** Dis;
    int NDis;
    bool saveOrNot = true;     // check if save or not

    public:
    Histogram(Clock& _ck, IOControl& _io, Parameter& _para);
    void initDistr();
    void addDistr(int & _Name, std::string _Type, std::string _Des,  double _a, double _b, unsigned long _size);
    void obsAdd(int _NDis, double _obs);
    void obsDel(int _NDis, double _obs);
    void obsPush(int _NDis, unsigned long _index, double _obs);
    void obsPull(int _NDis, unsigned long _index, double _obs);
    void saveDistr();
};
