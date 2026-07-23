#include "HistogramBase.hpp"


HistogramBase::HistogramBase(std::string _Type, std::string _Des, double _a, double _b, unsigned long _size, Clock& _ck, IOControl& _io) 
: ck(_ck), io(_io)
{
	if (_size == 0) throw std::invalid_argument("histogram size must be positive");
    Des = _Des;
    disCount = 0;
    disNum = _size;
    disMax = 0;
    disMin = disNum;

    a = _a;
    b = _b;

    if (_Type == "linear")
    {
		if (!std::isfinite(a) || !std::isfinite(b) || !(a < b))
			throw std::invalid_argument("linear histogram requires a < b");
        disType = 0;
    }
    else if (_Type == "log")
    {
        disType = 1;
        mappingArray.resize(disNum);
        unsigned long val = 0; 
        for (unsigned long i = 0; i < disNum; i++)
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
    if (disType == 0)
    {
		if (!std::isfinite(_obs) || _obs < a || _obs > b) return disNum;
		if (_obs == b) return disNum - 1;
        return static_cast<unsigned long>((_obs - a) * disNum / (b - a));
    }
    else if (disType == 1)
    {
		if (!std::isfinite(_obs) || mappingArray.empty() || _obs < mappingArray.front()) return disNum;
		std::vector<int>::const_iterator upper = std::upper_bound(mappingArray.begin(), mappingArray.end(), _obs);
		if (upper == mappingArray.end()) return disNum - 1;
		return static_cast<unsigned long>(upper - mappingArray.begin() - 1);
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
    unsigned long bindex = obs2Bin(_obs);

	if (bindex >= disNum) return;
	disCount++;
    disData[bindex] += 1;

    if(disMax < bindex) disMax = bindex;
    if(disMin > bindex) disMin = bindex;
}

void HistogramBase::disDel(double _obs)
{
    unsigned long bindex = obs2Bin(_obs);

	if (bindex >= disNum || disCount == 0) return;
	disCount--;
    disData[bindex] -= 1;

    if(disMax < bindex) disMax = bindex;
    if(disMin > bindex) disMin = bindex;
}

void HistogramBase::disPush(unsigned long _index, double _obs)
{
    if (_index >= disNum) return;
	disCount++;
    disData[_index] += _obs;

    if(disMax < _index) disMax = _index;
    if(disMin > _index) disMin = _index;
}

void HistogramBase::disPull(unsigned long _index, double _obs)
{
    if (_index >= disNum || disCount == 0) return;
	disCount--;
    disData[_index] -= _obs;

    if(disMax < _index) disMax = _index;
    if(disMin > _index) disMin = _index;
}

void HistogramBase::saveDis()
{
    if (!saveOrNot) return;

    double obsVal, obsNum, obsDis;

    std::string disPrint = "# disCount = " + toStr(disCount) + "\n";
	if (disCount == 0 || disMin >= disNum)
	{
		std::ofstream empty_file(Des + ".his");
		empty_file << disPrint;
		return;
	}
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
