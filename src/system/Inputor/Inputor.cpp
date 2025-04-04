#include "Inputor.hpp"


Inputor::Inputor(IOControl& _io, Parameter& _para, std::string input_name) : io(_io), para(_para)
{
    InFile = input_name;
    InpGroup = nullptr;
    NGroup = 0;
    initInputor();
    initSystemInputor();
    readInputor();
    io.exportInfo(io.OuputInfo, exportInputor());
}

#include "../../config/Inputor_config.hpp"

void Inputor::setGroup(std::string key, std::string des)
{
    InputorGroup** InpGroup0 = new InputorGroup* [NGroup + 1];
    for (int i = 0; i < NGroup; i++)
    {
        InpGroup0[i] = InpGroup[i];
    }
    InpGroup0[NGroup] = new InputorGroup(io, key, des);
    delete[] InpGroup;
    InpGroup = InpGroup0;
    NGroup++;
}

std::string Inputor::exportInputor()
{
    std::string str = "";
    for (int i = 0; i < NGroup; i++)
    {
        str += InpGroup[i]->exportInputor();
    }
    return str;
}

void Inputor::readInputor()
{
    std::ifstream IFStream;
    std::string Line;
    IFStream.open(InFile.c_str());

    if (!IFStream)
    {
        std::string str = "Warning from Inputor::readInputor():\n";
        str += "    No input file named \"" + InFile + "\"! Read data from initial data.\n";
        io.exportInfo(io.WarniInfo, str);
        return;
    }

    int LineNum = 0;
    std::stringstream SStream;
    std::string key;
    std::string temp;
    int iGroup = -1;
    int i, num;

    while (true)
    {
        if (!getline(IFStream, Line)) break;          // the end of the file
        LineNum++;        
        if (Line == "\r") continue;
        if (Line == "\n") continue;
        if (Line == "") continue;
        if (Line.substr(0, 5) == "//---")             // read this type string
        {   
            SStream.clear();
            SStream.str(Line);
            key = "";
            SStream >> temp >> key;
            num = 0;
            for (i = 0; i < NGroup; i++)
            {
                if (key == InpGroup[i]->getKeyWord())
                {
                    iGroup = i;
                    num++;
                }
            }
            if (num == 0)
            {
                std::string str = "Warning from Inputor::readInputor():\n";
                if (iGroup != -1)
                {
                    str += "    Invalid Inputor Group \"" + key + "\"! Use the last one \"" + InpGroup[iGroup]->getKeyWord() + "\"\n";
                }
                else
                {
                    str += "    Invalid Inputor Group \"" + key + "\"! Use the first one \"" + InpGroup[0]->getKeyWord() + "\"\n";
                }
                io.exportInfo(io.WarniInfo, str);
            }
            else if (num > 1)
            {
                std::string str = "Warning from Inputor::readInputor():\n";
                str += "    Multiple Inputor Groups have the same std::string \"" + key + "\"\n";
                io.exportInfo(io.WarniInfo, str);
            }
        }
        else
        {
            if (iGroup == -1)
            {
                std::string str = "Warning from Inputor::readInputor():\n";
                str += "    No valid Inputor Group! Use the first one \"" + InpGroup[0]->getKeyWord() + "\"\n";
                io.exportInfo(io.WarniInfo, str);
                iGroup = 0;
            }

            // read each term
            InpGroup[iGroup]->readInputor(Line);
        }
    }
}

void Inputor::initSystemInputor()
{
    // This is basic Monte Carlo input information, so we leave it in the system file.

    setGroup("Simulation_Parameters");
    addInputor(para.Seed      ,    "Seed"       , 		 1234       );
    addInputor(para.N_Measure ,    "N_Measure"  ,        1          );
    addInputor(para.N_Each    ,    "N_Each"     ,        100000     );
    addInputor(para.N_Therm   ,    "N_Therm"    ,        100        );
    addInputor(para.N_Total   ,    "N_Total"    ,        10000      );
    addInputor(para.NBlock    ,    "NBlock"     ,        1000       );
    addInputor(para.MaxNBin   ,    "MaxNBin"    ,        1000000    );
    addInputor(para.NperBin   ,    "NperBin"    ,        1          );
    
}

