#include "BasicObservable.hpp"


void BasicObservable::calAve()
{
	unsigned long i;
	Ave = 0;
    for (i = 0; i < NBlock; i++) Ave += BlkList[i];
    Ave = Ave / NBlock;
}

void BasicObservable::reset()
{
	unsigned long i;
	Value = 0;
	for (i = 0; i < NBlock; i++) BlkList[i] = 0;
	for (i = 0; i < MaxNBin; i++) BinList[i] = 0;
	Ave = 0;
	Err = 1e100;
	Cor = 0;
	
}

BasicObservable::BasicObservable(std::string _Name, std::string _Des, double _A, double _B, unsigned long _NBlock, unsigned long _MaxNbin, unsigned long _NperBin)
:ObservableBase(_Name, _Des, _A, _B, _NBlock, _MaxNbin, _NperBin)
{
	unsigned long i;
	Value = 0;
	BinList = new double[MaxNBin];
	for (i = 0; i < MaxNBin; i++)
	{
		BinList[i] = 0;
	}
}

void BasicObservable::measure(double value)
{
	Value = value;
}

void BasicObservable::collectData(unsigned long _NBin)
{
	BinList[_NBin] += Value;
}

void BasicObservable::calBlock(unsigned long _NBin, unsigned long _NperBin)
{
	unsigned long n = _NBin / NBlock;
	unsigned long i, j = 0, k = 0;
	for (i = 0; i < NBlock; i++)
	{
		BlkList[i] = 0;
		for (j = 0; j < n; j++)
		{
			BlkList[i] += BinList[k];
			k++;
		}
		BlkList[i] = BlkList[i] / n / _NperBin;
	}
}

std::string BasicObservable::printCurrent(int Index)
{
	char a[150];
    std::string str;
    sprintf_s(a,"%-4d %-12.12s = % -16.8e  %-40.40s\n",Index,Name.c_str(),A+B*Value,Des.c_str());
    str = a;
    return  str;
}

void BasicObservable::compressBin()
{
	unsigned long i;
	unsigned long maxnb2 = MaxNBin / 2;
	for (i = 0; i < maxnb2; i++)
	{
		BinList[i] = BinList[2 * i] + BinList[2 * i + 1];
	}
	for (i = maxnb2; i < MaxNBin; i++)
	{
		BinList[i] = 0;
	}
}