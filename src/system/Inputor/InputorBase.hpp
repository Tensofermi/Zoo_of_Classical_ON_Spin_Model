#pragma once
#include <bits/stdc++.h>
#include "../Utils/Utils.hpp"

class InputorBase
{
public:
    InputorBase() {}
    virtual ~InputorBase() {}
    virtual int readInputor(std::string Line) { return 0; }
    virtual std::string exportInputor() { return ""; }
};
