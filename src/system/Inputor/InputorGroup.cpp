#include "InputorGroup.hpp"

void InputorGroup::readInputor(std::string Line)
{
    int num = 0;
    for (int i = 0; i < NInp; i++)
    {
        num += Inp[i]->readInputor(Line);
    }
    if (num == 0)
    {
        std::string str = "Warning from Inputor::readInputor():\n";
        str += "    No Terms match the std::string!!!\n";
        io.exportInfo(io.WarniInfo, str);
    }
    else if (num > 1)
    {
        std::string str = "Warning from Inputor::readInputor():\n";
        str += "    Multiple Terms match the std::string!!!\n";
        io.exportInfo(io.WarniInfo, str);
    }
}

InputorGroup::InputorGroup(IOControl& _io, std::string key, std::string des) : io(_io)
{
    NInp = 0;
    Inp = nullptr;
    Key = key;
    Des = des;
}


std::string InputorGroup::exportInputor()
{
    std::string str = "//----- " + Key + "  \n";
    for (int i = 0; i < NInp; i++)
    {
        str += Inp[i]->exportInputor() + "\n";
    }
    str = str + "\n";
    return str;
}