#include "Hypercubic.hpp"


//initialization
void Hypercubic::set(int _Dim, int _L)
{
    Dim = _Dim;
    L = _L;
    Vol = pow(L, Dim);
    NNb = 2 * Dim;

    x_ = new int[Dim];
}

int Hypercubic::getL()
{
    return L;
}

long Hypercubic::getVol()
{
    return Vol;
}

long Hypercubic::getNBond()
{
    return Dim * Vol;
}

int Hypercubic::getNNb()
{
    return NNb;
}

int Hypercubic::getDim()
{
    return Dim;
}

int Hypercubic::getOpsDir(int _Dir)
{
    return NNb - _Dir - 1;
}

//-----------------------------------------------------------
//-----------------------------------------------------------
int Hypercubic::getComponent(long _Site, int _Dir)      // Site --> (x_n, x_n-1, x_n-2, ...)
{
    for (int i = Dim - 1; i > _Dir; i--) _Site = _Site / L;
    return _Site % L;
}

long Hypercubic::getSite(int *_Component)               // (x_n, x_n-1, x_n-2, ...) --> Site
{
    long Vc;
    Vc = _Component[0];
    for (int i = 1; i < Dim; i++)
    {
        Vc = _Component[i] + Vc * L;
    }
    return Vc;
}

long Hypercubic::getSite(std::vector<int> &_Component)  // (x_n, x_n-1, x_n-2, ...) --> Site
{
    long Vc;
    Vc = _Component[0];
    for (int i = 1; i < Dim; i++)
    {
        Vc = _Component[i] + Vc * L;
    }
    return Vc;
}

//-----------------------------------------------------------
//-----------------------------------------------------------

long Hypercubic::getNNSite(long _Site, int _Dir)
{
    int i;
    for (i = 0; i < Dim; i++) x_[i] = getComponent(_Site, i);
    if(_Dir < Dim)
    {
        if((++x_[_Dir])>=L) x_[_Dir] -= L;
    }
    else
    {
        _Dir = getOpsDir(_Dir);
        if((--x_[_Dir])<0) x_[_Dir] += L;
    }
    return getSite(x_);
}

long Hypercubic::getNNBond(long _Site, int _Dir)
{
    if(_Dir < Dim) {return _Site + _Dir * Vol;}
    else {return getNNSite(_Site, _Dir) + getOpsDir(_Dir) * Vol;}
}

int Hypercubic::getDir(long _Site, long _NNSite)
{
    for (int Dir = 0; Dir < NNb; Dir++)
    {
        if(getNNSite(_Site, Dir)==_NNSite) return Dir;
    }
    return -1;
}

int Hypercubic::getParity(long _Site)
{
	long x = 0;
	for (int i = 0; i < Dim; i++)
	{
		x += getComponent(_Site, i);
	}
	if(x % 2) return -1;
	return 1;
}

long Hypercubic::getDirSite(long _Site, int _Dir, int _Length)
{
    int i;
    for (i = 0; i < Dim; i++) x_[i] = getComponent(_Site, i);
    if(_Dir < Dim)
    {
        x_[_Dir] += _Length; 
        if((x_[_Dir])>=L) x_[_Dir] -= L;
    }
    else
    {
        _Dir = getOpsDir(_Dir);
        x_[_Dir] -= _Length; 
        if((x_[_Dir])<0) x_[_Dir] += L;
    }
    return getSite(x_);

}


