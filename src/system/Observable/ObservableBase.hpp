#pragma once
#include <bits/stdc++.h>
#include "../Utils/Utils.hpp"
#include "../Clock/Clock.hpp"
#include "../IOControl/IOControl.hpp"
#include "../../config/Parameter_config.hpp"
#define sprintf_s sprintf

class ObservableBase
{
public:
	unsigned long NBlock;
	unsigned long MaxNBin;
	unsigned long NperBin;
	
	std::string Name;
	std::string Des;
	double* BlkList;
	double Ave;
	double Err;
	double Cor;
	double A;
	double B;

public:
	ObservableBase();
	ObservableBase(std::string _Name, std::string _Des, double _A, double _B, unsigned long _NBlock, unsigned long _MaxNbin, unsigned long _NperBin);
	void calErr();
	std::string printAverage(int Index);
	double getBlock(int i);
	double getAve();
};