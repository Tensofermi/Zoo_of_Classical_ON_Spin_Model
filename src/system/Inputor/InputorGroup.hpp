#pragma once
#include <bits/stdc++.h>
#include "../Utils/Utils.hpp"
#include "InputorTerm.hpp"
#include "../IOControl/IOControl.hpp"
#define sprintf_s sprintf

class InputorGroup
{
    InputorBase** Inp;
    int NInp;
    std::string Key;
    std::string Des;

    IOControl& io;

public:
    InputorGroup(IOControl& _io, std::string key, std::string des);
    template <typename T1, typename T2>
    void addInputor(T1& variable, std::string key, T2 initVar, std::string des);
    std::string exportInputor();
    void readInputor(std::string Line);
    std::string getKeyWord() { return Key; }
};

template <typename T1, typename T2>
void InputorGroup::addInputor(T1& variable, std::string key, T2 initVar, std::string des)
{
    InputorBase** Inp0 = new InputorBase * [NInp + 1];
    for (int i = 0; i < NInp; i++)
    {
        Inp0[i] = Inp[i];
    }
    Inp0[NInp] = new InputorTerm<T1>(variable, key, initVar, des);
    delete[] Inp;
    Inp = Inp0;
    NInp++;
}