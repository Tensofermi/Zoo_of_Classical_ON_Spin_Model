#pragma once
#include <bits/stdc++.h>
#include "../Utils/Utils.hpp"
#include "../IOControl/IOControl.hpp"
#include "../../config/Parameter_config.hpp"
#include "InputorGroup.hpp"
#define sprintf_s sprintf

class Inputor
{
    std::string InFile;  // name of input file

    InputorGroup** InpGroup;
    int NGroup;

    IOControl& io;
    Parameter& para;

  public:
    Inputor(IOControl& _io, Parameter& _para, std::string name = "input.txt");   // default name is "input.txt"
    void initInputor();
    void initSystemInputor();
    void readInputor();             // Key function !!
    std::string exportInputor();

    void setGroup(std::string key, std::string des = "");
    
    template <typename T1, typename T2>
    void addInputor(T1& variable, std::string key, T2 initVar, std::string des = "");
    
};

template <typename T1, typename T2>
void Inputor::addInputor(T1& variable, std::string key, T2 initVar, std::string des)
{
    InpGroup[NGroup - 1]->addInputor(variable, key, initVar, des);
}