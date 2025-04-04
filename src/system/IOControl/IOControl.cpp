#include "IOControl.hpp"

void IOControl::clearInfo()   // delete the last files
{
    void wrt(std::string File, std::string Info);
    for (int i = 0; i < ExportInfoNum; i++) remove(Files[i].c_str());
    return;
}

void IOControl::exportInfo(int InfoType, std::string Info)
{
    int x = Opers[InfoType];
    for (int i = 0; x != 0; i++, x = x / 2)
    {
      if (x % 2 == 1) ExcuteFunc[i](Files[InfoType], Info);
    }
}

#include "../../config/IOControl_config.hpp"

IOControl::IOControl()
{
    void noPrtOrWrt(std::string File, std::string Info);
    void prt(std::string File, std::string Info);
    void wrt(std::string File, std::string Info);
    void hisWrt(std::string File, std::string Info);
    void abort(std::string File, std::string info);

    // File names and operations for each file
    Files = new std::string[ExportInfoNum];
    Opers = new int[ExportInfoNum];

    // Basic operations for IO control
    NoPrtOrWrt = (int)pow(2, 0);
           Prt = (int)pow(2, 1);
           Wrt = (int)pow(2, 2);
        HisWrt = (int)pow(2, 3);
         Abort = (int)pow(2, 4);

    ExcuteFunc.push_back(noPrtOrWrt);
    ExcuteFunc.push_back(prt);
    ExcuteFunc.push_back(wrt);
    ExcuteFunc.push_back(hisWrt);
    ExcuteFunc.push_back(abort);

    initIOControl();
    clearInfo();
}

IOControl::~IOControl()
{
    delete[] Opers; Opers = nullptr; 
    delete[] Files; Files = nullptr; 
}

//--------------------------------
//--- Functions for IO control
//--------------------------------
void noPrtOrWrt(std::string File, std::string Info)
{
    return;
}

void prt(std::string File, std::string Info)
{
    std::cout << Info;
    return;
}

void wrt(std::string File, std::string Info)
{
    std::ofstream file;
    file.open(File.c_str());
    file << Info;
    file.close();
    return;
}

void hisWrt(std::string File, std::string Info)
{
    std::ofstream file;
    file.open(File.c_str(), std::ios::app);
    file << Info;
    file.close();
    return;
}

void abort(std::string File, std::string info)
{
    std::string str = "";
    str += "Attention!\n    This program is aborted, see details in file\"" + File + "\"\n";
    std::cout << str << std::endl;
    std::ofstream file;
    file.open(File.c_str(), std::ios::app);
    file << info;
    file.close();
    exit(0);
}