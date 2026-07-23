#pragma once
#include "common.h"
#include "../Utils/Utils.hpp"
#include "InputorTerm.hpp"
#include "../IOControl/IOControl.hpp"
class InputorGroup
{
    std::vector<std::unique_ptr<InputorBase> > Inp;
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
    Inp.emplace_back(new InputorTerm<T1>(variable, key, initVar, des));
    NInp++;
}
