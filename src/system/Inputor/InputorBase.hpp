#pragma once
#include <bits/stdc++.h>
#include "../Utils/Utils.hpp"
#define sprintf_s sprintf

class InputorBase
{
public:
    InputorBase() {}
    virtual int readInputor(std::string Line) { return 0; }
    virtual std::string exportInputor() { return ""; }
};