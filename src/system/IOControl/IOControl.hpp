#pragma once
#include <bits/stdc++.h>
#include "../Utils/Utils.hpp"

class IOControl
{
    std::vector<void(*)(std::string File, std::string Info)> ExcuteFunc;

    int* Opers;
    std::string* Files;

    int NoPrtOrWrt;     // do nothing
    int Prt;            // print data in terminal
    int Wrt;            // rewrite data in file
    int HisWrt;         // append data in file
    int Abort;          // abort program

public:
    // the number of total types
    int ExportInfoNum = 6;

    int InputInfo = 0;	// 	check input parameter
    int ErrorInfo = 1;	//	error ocurr
    int WarniInfo = 2;	//	warning ocurr
    int TestiInfo = 3;	//	for test 
    int OuputInfo = 4;	//	output routine 
    int OuputCont = 5;	//	output current obs

    IOControl();
    ~IOControl();
    void initIOControl();
    void clearInfo();

    void exportInfo(int InfoType, std::string Info);
};
