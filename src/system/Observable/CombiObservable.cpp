#include "CombiObservable.hpp"


CombiObservable::CombiObservable(std::string _Name, std::string _Des, double _A, double _B, unsigned long _NBlock, unsigned long _MaxNbin, unsigned long _NperBin)
:ObservableBase(_Name, _Des, _A, _B, _NBlock, _MaxNbin, _NperBin)
{
    
}

void CombiObservable::calAve(double _Value)
{
	Ave = _Value;
}

void CombiObservable::calBlock(int _BlockIndex, double _Value)
{
	BlkList[_BlockIndex] = _Value;
}

void CombiObservable::reset()
{
	unsigned long i;
	for (i = 0; i < NBlock; i++) BlkList[i] = 0;
	Ave = 0;
	Err = 1e100;
	Cor = 0;

}
