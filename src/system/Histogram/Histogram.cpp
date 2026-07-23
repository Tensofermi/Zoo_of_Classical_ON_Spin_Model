#include "Histogram.hpp"


Histogram::Histogram(Clock& _ck, IOControl& _io, Parameter& _para) : ck(_ck), io(_io), para(_para)
{
    NDis = 0;
    initDistr();
}

#include "../../config/Histogram_config.hpp"

void Histogram::addDistr(int & _Name, std::string _Type, std::string _Des,  double _a, double _b, unsigned long _size)
{
    _Name = NDis;
    
    Dis.emplace_back(new HistogramBase(_Type, _Des, _a, _b, _size, ck, io));
    NDis++;
}


void Histogram::obsAdd(int _NDis, double _obs)
{
    Dis[_NDis]->disAdd(_obs);
}

void Histogram::obsDel(int _NDis, double _obs)
{
    Dis[_NDis]->disDel(_obs);
}

void Histogram::obsPush(int _NDis, unsigned long _index, double _obs)
{
    Dis[_NDis]->disPush(_index, _obs);
}

void Histogram::obsPull(int _NDis, unsigned long _index, double _obs)
{
    Dis[_NDis]->disPull(_index, _obs);
}

void Histogram::saveDistr()
{
    for (int i = 0; i < NDis; i++)
    {
        Dis[i]->saveDis();
    }
}
