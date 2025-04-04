#include "Observable.hpp"


void Observable::reset()
{
	unsigned int Index;
	for (Index = 0; Index < NBasic; Index++) BasicObser[Index]->reset();
	for (Index = 0; Index < NCombi; Index++) CombiObser[Index]->reset();
	NperBin = 1;
	NBin = 0;
	IBin = 0;
	MCStep = 0;
}


Observable::Observable(Clock& _ck, IOControl& _io, Parameter& _para) : ck(_ck), io(_io), para(_para)
{
	NBasic = 0;
	NCombi = 0;
	BasicObser = nullptr;
	CombiObser = nullptr;
	NBlock = para.NBlock;
	MaxNBin = para.MaxNBin;
	NperBin = para.NperBin;

	initObservable();
}

#include "../../config/Observable_config.hpp"

void Observable::calAveErr()
{
	unsigned int i;
	// get Ave
	calBlock();
	// get Err
	for (i = 0; i < NBasic; i++) BasicObser[i]->calErr();
	for (i = 0; i < NCombi; i++) CombiObser[i]->calErr();
}

void Observable::addBasicObser(int &_Index, std::string _Name, std::string _Des, double _A, double _B)
{
	BasicObservable** BasicObser0 = new BasicObservable*[NBasic+1];
	_Index = NBasic;
	for (unsigned int i = 0; i < NBasic; i++) BasicObser0[i] = BasicObser[i];
    BasicObser0[NBasic] = new BasicObservable(_Name, _Des, _A, _B, NBlock, MaxNBin, NperBin);
	delete [] BasicObser;
	BasicObser = BasicObser0;
	NBasic++;
	Ob.clear();
	Ob.resize(NBasic);
}

void Observable::addCombiObser(int &_Index, std::string _Name, std::string _Des, double _A, double _B)
{
	CombiObservable** CombiObser0 = new CombiObservable*[NCombi+1];
	_Index = NCombi;
	for (unsigned int i = 0; i < NCombi; i++) CombiObser0[i] = CombiObser[i];
    CombiObser0[NCombi] = new CombiObservable(_Name, _Des, _A, _B, NBlock, MaxNBin, NperBin);
	delete [] CombiObser;
	CombiObser = CombiObser0;
	NCombi++;
	Result.clear();
	Result.resize(NCombi);
}

std::string Observable::printCurrent()
{
	unsigned int i;
	std::string str = "";
	for (i = 0; i < NBasic; i++)
	{
		str += BasicObser[i]->printCurrent(i);
	}
	return str;
}

std::string Observable::printAverage()
{
	unsigned int i;
	std::string str = "";
	calAveErr();
	for (i = 0; i < NBasic; i++)
	{
		str += BasicObser[i]->printAverage(i);
	}
	for (i = 0; i < NCombi; i++)
	{
		str += CombiObser[i]->printAverage(i+NBasic);
	}
	str += "NperBin = " + toStr(NperBin) + " , NperBlk = " + toStr(NperBin * NBin / NBlock) + " , NBin = " + toStr(NBin) + "\n";
	return str;
}

void Observable::calBlock()
{
	unsigned long i, index;
	
	//--- get BasicObser Ave and BlkList
	for (index = 0; index < NBasic; index++)
	{
		BasicObser[index]->calBlock(NBin, NperBin);
		BasicObser[index]->calAve();
	}

	//--- get CombiObser Ave
	for (index = 0; index < NBasic; index++) Ob[index] = BasicObser[index]->getAve();
	calCombiObser();
	for (index = 0; index < NCombi; index++) CombiObser[index]->calAve(Result[index]);

	//--- get CombiObser BlkList
	for (i = 0; i < NBlock; i++)
	{
		for (index = 0; index < NBasic; index++) Ob[index] = BasicObser[index]->getBlock(i);
		calCombiObser();
		for (index = 0; index < NCombi; index++) CombiObser[index]->calBlock(i,Result[index]);		
	}
}

// ---------------------------------------------------------
// This function adds the current obs value to the bin list.
// ---------------------------------------------------------
void Observable::collectData()
{
	unsigned int i;
	for (i = 0; i < NBasic; i++)
	{
		BasicObser[i]->measure(Ob[i]);	    // send Ob[i] to BasicObser[i]
		BasicObser[i]->collectData(NBin);	// add the Value to BinList[Nbin]
	}
	IBin++;		// The index for one Bin
	if(IBin == NperBin)
	{
		IBin = 0;
		NBin++;
		if (NBin == MaxNBin)	// overflow
		{
			for (i = 0; i < NBasic; i++)
			{
				BasicObser[i]->compressBin();
			}
			NBin = MaxNBin / 2;
			NperBin = 2 * NperBin;
		}
		if(NBin%NBlock == 0) MCStep1 = MCStep;
	}
	MCStep0 = MCStep;
}
