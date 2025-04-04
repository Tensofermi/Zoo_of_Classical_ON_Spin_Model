#pragma once
#include <bits/stdc++.h>
#include "../Utils/Utils.hpp"
#include "../Clock/Clock.hpp"
#include "../IOControl/IOControl.hpp"
#include "../../config/Parameter_config.hpp"
#include "BasicObservable.hpp"
#include "CombiObservable.hpp"
#define sprintf_s sprintf

class Observable
{
	Clock& ck;
    IOControl& io;
    Parameter& para;

public:
	unsigned int NBasic;
	unsigned int NCombi;
	BasicObservable** BasicObser;
	CombiObservable** CombiObser;
	std::vector<double> Ob;
	std::vector<double> Result;

	double MCStep = 0;
	double MCStep0 = 0;
	double MCStep1 = 0;
	double ZZ = 0;
	double ZZ0 = 0;

	unsigned long NBin = 0;
	unsigned long IBin = 0;

	unsigned long NBlock = 1000;
	unsigned long MaxNBin = 100000;
	unsigned long NperBin = 1;
	
public:
	Observable(Clock& _ck, IOControl& _io, Parameter& _para);
	void addBasicObser(int &_Index, std::string _Name, std::string _Des, double _A, double _B);
	void addCombiObser(int &_Index, std::string _Name, std::string _Des, double _A, double _B);
	void initObservable();
	void calCombiObser();
	void collectData();
	void calBlock();
	void calAveErr();
	void reset();
	std::string printOutput(bool Flag);
	std::string printCurrent();
	std::string printAverage();
};
