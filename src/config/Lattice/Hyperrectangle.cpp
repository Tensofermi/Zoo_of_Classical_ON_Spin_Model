#include "Hyperrectangle.hpp"


// Initialization
void Hyperrectangle::set(int _Dim, std::initializer_list<int> initList)
{
    Dim = _Dim;
    L.assign(initList); // 使用assign将初始化列表赋给成员变量L
    Vol = 1;
    for (int i = 0; i < Dim; ++i)
        Vol *= L[i];

    NNb = 2 * Dim;
    x_ = new int[Dim];
}


long Hyperrectangle::getVol()
{
    return Vol;
}

long Hyperrectangle::getNBond()
{
    return Dim * Vol;
}

int Hyperrectangle::getNNb()
{
    return NNb;
}

int Hyperrectangle::getDim()
{
    return Dim;
}

int Hyperrectangle::getOpsDir(int _Dir)
{
    return NNb - _Dir - 1;
}

//-----------------------------------------------------------
//-----------------------------------------------------------
                                                            // _Dir --> (0,        1,    2, ...)
int Hyperrectangle::getComponent(long _Site, int _Dir)      // Site --> (x_n, x_n-1, x_n-2, ...)
{
    for (int i = Dim - 1; i > _Dir; i--) _Site = _Site / L[i];
    return _Site % L[_Dir];
}

long Hyperrectangle::getSite(int *_Component)               // (x_n, x_n-1, x_n-2, ...) --> Site
{
    long Vc;
    Vc = _Component[0];
    for (int i = 1; i < Dim; i++)
    {
        Vc = _Component[i] + Vc * L[Dim-i];
    }
    return Vc;
}

long Hyperrectangle::getSite(std::vector<int> &_Component)  // (x_n, x_n-1, x_n-2, ...) --> Site
{
    long Vc;
    Vc = _Component[0];
    for (int i = 1; i < Dim; i++)
    {
        Vc = _Component[i] + Vc * L[Dim-i];
    }
    return Vc;
}

//-----------------------------------------------------------
//-----------------------------------------------------------

long Hyperrectangle::getNNSite(long _Site, int _Dir)
{
    for (int i = 0; i < Dim; i++) x_[i] = getComponent(_Site, i);
    if(_Dir < Dim)
    {
        if((++x_[_Dir])>=L[_Dir]) x_[_Dir] -= L[_Dir];
    }
    else
    {
        _Dir = getOpsDir(_Dir);
        if((--x_[_Dir])<0) x_[_Dir] += L[_Dir];
    }
    return getSite(x_);
}

long Hyperrectangle::getNNBond(long _Site, int _Dir)
{
    if(_Dir < Dim) {return _Site + _Dir * Vol;}
    else {return getNNSite(_Site, _Dir) + getOpsDir(_Dir) * Vol;}
}

int Hyperrectangle::getDir(long _Site, long _NNSite)
{
    for (int Dir = 0; Dir < NNb; Dir++)
    {
        if(getNNSite(_Site, Dir)==_NNSite) return Dir;
    }
    return -1;
}

int Hyperrectangle::getParity(long _Site)
{
	long x = 0;
	for (int i = 0; i < Dim; i++)
	{
		x += getComponent(_Site, i);
	}
	if(x % 2) return -1;
	return 1;
}

long Hyperrectangle::getDirSite(long _Site, int _Dir, int _Length)
{
    for (int i = 0; i < Dim; i++) x_[i] = getComponent(_Site, i);
    if(_Dir < Dim)
    {
        x_[_Dir] += _Length; 
        if((x_[_Dir])>=L[_Dir]) x_[_Dir] -= L[_Dir];
    }
    else
    {
        _Dir = getOpsDir(_Dir);
        x_[_Dir] -= _Length; 
        if((x_[_Dir])<0) x_[_Dir] += L[_Dir];
    }
    return getSite(x_);

}


