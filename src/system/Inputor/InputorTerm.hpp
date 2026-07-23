#pragma once
#include <bits/stdc++.h>
#include "../Utils/Utils.hpp"
#include "InputorBase.hpp"
template <typename T>
class InputorTerm : public InputorBase
{
    T* Var;
    std::string Key;
    int Flag;
    std::string Des;

public:
    InputorTerm(T& variable, std::string key, T initVar, std::string des);
    int readInputor(std::string Line);      // read parameters and check if valid
    std::string exportInputor();            // output what is read by program
};

template <typename T>
InputorTerm<T>::InputorTerm(T& variable, std::string key, T initVar, std::string des)
    : Var(&variable), Key(key), Flag(0), Des(des)
{
    *Var = initVar;
}

template <typename T>
int InputorTerm<T>::readInputor(std::string Line)
{
    std::stringstream stream;
    std::string key;
    stream << Line;
    stream >> key;
    if (key == Key)
    {
        T value;
        if (!(stream >> value))
        {
            return -1;
        }
        *Var = value;
        Flag++;
        return 1;
    }
    return 0;
}

template <typename T>
std::string InputorTerm<T>::exportInputor()
{
    std::stringstream stream;
    std::string flag, datastr;
    char a[128];
    std::string printstr;
    stream << *Var;
    stream >> datastr;
    switch (Flag) {
    case 0:
        flag = "Dflt";
        break;
    case 1:
        flag = "Read";
        break;
    default:
        flag = "Last";
    }
    std::snprintf(a, sizeof(a), "%-12.12s%-20.20s%-6.6s%-40.40s", Key.c_str(), datastr.c_str(), flag.c_str(), Des.c_str());
    printstr = a;
    return printstr;
}
