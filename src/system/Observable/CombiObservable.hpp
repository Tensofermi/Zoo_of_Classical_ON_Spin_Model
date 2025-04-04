#pragma once
#include <bits/stdc++.h>
#include "../Utils/Utils.hpp"
#include "../Clock/Clock.hpp"
#include "../IOControl/IOControl.hpp"
#include "../../config/Parameter_config.hpp"
#include "ObservableBase.hpp"
#define sprintf_s sprintf

class CombiObservable :public ObservableBase
{
public:
	CombiObservable(std::string _Name, std::string _Des, double _A, double _B, unsigned long _NBlock, unsigned long _MaxNbin, unsigned long _NperBin);
	void calBlock(int _BlockIndex, double _Value);
	void reset();
	void calAve(double _Value);
};