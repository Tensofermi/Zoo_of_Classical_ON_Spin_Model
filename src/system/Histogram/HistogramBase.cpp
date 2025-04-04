#include "HistogramBase.hpp"


HistogramBase::HistogramBase(std::string _Type, std::string _Des, double _a, double _b, unsigned long _size, Clock& _ck, IOControl& _io) 
: ck(_ck), io(_io)
{
    Des = _Des;
    disCount = 0;
    disNum = _size;
    disMax = 0;
    disMin = disNum;

    a = _a;
    b = _b;

    if (_Type == "linear")
    {
        disType = 0;
    }
    else if (_Type == "log")
    {
        disType = 1;
        mappingArray.resize(logNum);
        unsigned long val = 0; 
        for (int i = 0; i < logNum; i++)
        {
            mappingArray[i] = val;
            val += interval;
            if (i != 0 && i % modsteps == 0) interval = 2 * interval;
        }
    }
    else
    {
        io.exportInfo(io.ErrorInfo, "something wrong in Histogram type");
    }

    disData.resize(disNum);
    for (unsigned long i = 0; i < disNum; i++)
        disData[i] = 0;
}

unsigned long HistogramBase::obs2Bin(double _obs)
{
    unsigned long Bin_;

    if (disType == 0)
    {
        // mapping [a,b] into [0,disNum]
        Bin_ = (long)((_obs - a) * disNum / (b - a));
        return Bin_;
    }
    else if (disType == 1)
    {
        int left, right;
        left = 0;
        right = logNum - 1;
        while (left <= right)
        {
            Bin_ = left + (right - left) / 2;
            if (_obs == mappingArray[Bin_]) return Bin_;
            else if (mappingArray[Bin_] < _obs) left = Bin_ + 1;
            else right = Bin_ - 1;
        }

    }

    return 0;
}

double HistogramBase::bin2Obs(unsigned long _bin)
{
    double Obs_;

    if (disType == 0)
    {
        // mapping [0,disNum] into [a,b]
        Obs_ = a + (b - a) * _bin / disNum;
        return Obs_;
    }
    else if (disType == 1)
    {
        // return log function
        return mappingArray[_bin];
    }

    return 0;
}

void HistogramBase::disAdd(double _obs)
{
    disCount++;
    unsigned long bindex = obs2Bin(_obs);

    if (bindex >= disNum || bindex == 0) return; // overflow
    disData[bindex] += 1;

    if(disMax < bindex) disMax = bindex;
    if(disMin > bindex) disMin = bindex;
}

void HistogramBase::disDel(double _obs)
{
    disCount--;
    unsigned long bindex = obs2Bin(_obs);

    if (bindex >= disNum || bindex == 0) return; // overflow
    disData[bindex] -= 1;

    if(disMax < bindex) disMax = bindex;
    if(disMin > bindex) disMin = bindex;
}

void HistogramBase::disPush(unsigned long _index, double _obs)
{
    disCount++;
    
    if (_index >= disNum || _index == 0) return; // overflow
    disData[_index] += _obs;

    if(disMax < _index) disMax = _index;
    if(disMin > _index) disMin = _index;
}

void HistogramBase::disPull(unsigned long _index, double _obs)
{
    disCount--;
    
    if (_index >= disNum || _index == 0) return; // overflow
    disData[_index] -= _obs;

    if(disMax < _index) disMax = _index;
    if(disMin > _index) disMin = _index;
}

void HistogramBase::saveDis()
{
    if (!saveOrNot) return;

    double obsVal, obsNum, obsDis;

    std::string disPrint = "# disCount = " + toStr(disCount) + "\n";
    for (unsigned long i = disMin; i <= disMax; i++)
    {
        obsVal = bin2Obs(i);
        obsNum = disData[i];    
        obsDis = obsNum/(double)disCount;
        disPrint += l_jf(toStr(obsVal), 20);   //  observable value
        disPrint += l_jf(toStr(obsNum), 20);   //  counting number
        disPrint += l_jf(toStr(obsDis), 20);   //  normalization
        disPrint += "\n";
    }

    // save
	std::ofstream file;
	file.open(Des + ".his");
	file << disPrint;
	file.close();
}