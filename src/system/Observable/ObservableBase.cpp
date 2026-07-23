#include "ObservableBase.hpp"


ObservableBase::ObservableBase()
	: NBlock(0), MaxNBin(0), NperBin(0), Name("UNNAMED"),
	  Des("This Observable has not been initialized!"), Ave(0),
	  Err(std::numeric_limits<double>::infinity()), Cor(0), A(0), B(1)
{
}

ObservableBase::ObservableBase(std::string _Name, std::string _Des, double _A, double _B, unsigned long _NBlock, unsigned long _MaxNbin, unsigned long _NperBin)
{
	NBlock = _NBlock;
	MaxNBin = _MaxNbin;
	NperBin = _NperBin;

	Name = _Name;
	Des = _Des;
	A = _A;
	B = _B;
	if (NBlock < 2) throw std::invalid_argument("NBlock must be at least 2");
	if (MaxNBin == 0) throw std::invalid_argument("MaxNBin must be positive");
	if (NperBin == 0) throw std::invalid_argument("NperBin must be positive");
	BlkList.assign(NBlock, 0.0);
    Ave = 0;
    Err = 1e100;
    Cor = 0;
}

void ObservableBase::calErr()
{
	if (NBlock < 2 || BlkList.size() != NBlock)
		throw std::logic_error("Observable blocks are not initialized");
    unsigned long i;
    double err, cor, ave;
    double devn, devp;
    ave = Ave;
    
    err = 0; cor = 0;
    devn = 0; devp = 0;
    for (i = 0; i < NBlock; i++)
    {
        devp = devn;
        devn = BlkList[i] - ave;
        err += devn * devn;
        cor += devp * devn;
    }
	if(err < 1e-99)
	{
		Cor = 1e8;
	}
	else
	{
		Cor = cor / err;
	}
    Err = sqrt(err / static_cast<double>(NBlock) / static_cast<double>(NBlock - 1));
}

std::string ObservableBase::printAverage(int Index)
{
	char a[150];
	std::string str;
	std::snprintf(a, sizeof(a), "%-4d %-12.12s = % -16.8e % -3.8e % -3.3e  %-40.40s\n", Index, ("<"+Name+">").c_str(), A+B*Ave, fabs(B)*Err, Cor, Des.c_str());
    str = a;
	//--------------------------------------------------//
	//-------------------- save obs --------------------//
	//--------------------------------------------------//
	// std::string obs_save;
	// obs_save = r_jf(dou2str(A+B*Ave,9), 12) + "    " + r_jf(dou2str(fabs(B)*Err,9), 12) + "    " + r_jf(dou2str(Cor,9), 12) + "\n";
	// ofstream file;
	// // file.open((Name + ".dat").c_str());
	// file.open((Name + ".dat").c_str(), std::ios::app);
	// file << obs_save;
	// file.close();
	//--------------------------------------------------//
	//-------------------- save obs --------------------//
	//--------------------------------------------------//
    return str;
}

double ObservableBase::getBlock(int i)
{
	return BlkList.at(static_cast<std::size_t>(i));
}

double ObservableBase::getAve()
{
	return Ave;
}
