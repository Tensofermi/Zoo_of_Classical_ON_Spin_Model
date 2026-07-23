#pragma once
#include "common.h"
#include "../Utils/Utils.hpp"
#include "../Clock/Clock.hpp"
#include "../IOControl/IOControl.hpp"
#include "../../config/Parameter_config.hpp"
#include "ObservableBase.hpp"
class BasicObservable :public ObservableBase
{
public:
	double  Value;
		std::vector<double> BinList;

public:
	BasicObservable();
	BasicObservable(std::string _Name, std::string _Des, double _A, double _B, unsigned long _NBlock, unsigned long _MaxNbin, unsigned long _NperBin);
	void measure(double value);
	void collectData(unsigned long _NBin);
	void compressBin();
	std::string printCurrent(int Index);
	void calBlock(unsigned long _NBin, unsigned long _NperBin);
	void calAve();
	void reset();
};
