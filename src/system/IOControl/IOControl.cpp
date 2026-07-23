#include "IOControl.hpp"

void IOControl::clearInfo()   // delete the last files
{
    void wrt(std::string File, std::string Info);
    for (int i = 0; i < ExportInfoNum; i++) remove(Files[i].c_str());
    return;
}

void IOControl::exportInfo(int InfoType, std::string Info)
{
	if (InfoType < 0 || InfoType >= ExportInfoNum)
		throw std::out_of_range("invalid IOControl information type");
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
    void abortProgram(std::string File, std::string info);

    // File names and operations for each file
    Files.resize(ExportInfoNum);
    Opers.resize(ExportInfoNum);

    // Basic operations for IO control
    NoPrtOrWrt = 1 << 0;
           Prt = 1 << 1;
           Wrt = 1 << 2;
        HisWrt = 1 << 3;
         Abort = 1 << 4;

    ExcuteFunc.push_back(noPrtOrWrt);
    ExcuteFunc.push_back(prt);
    ExcuteFunc.push_back(wrt);
    ExcuteFunc.push_back(hisWrt);
    ExcuteFunc.push_back(abortProgram);

    initIOControl();
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

void abortProgram(std::string File, std::string info)
{
    std::string str = "";
    str += "Attention!\n    This program is aborted, see details in file\"" + File + "\"\n";
    std::cout << str << std::endl;
    std::ofstream file;
    file.open(File.c_str(), std::ios::app);
    file << info;
    file.close();
    std::exit(EXIT_FAILURE);
}
