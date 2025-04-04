#pragma once
#include <bits/stdc++.h>

class Hyperrectangle
{

    // 1D               2D                  3D                           ...
    //                          (x)                 (x) (-y)  
    //                           1                   2  4                  
    //                           |                   | /
    //                           |                   |/
    //  0 -------- 1      3 -----|----- 0     0 -----|---- 5 (-z)        ...
    // (x)       (-x)    (-y)    |     (y)   (z)    /|
    //                           |                 / |
    //                           2                1  3
    //                         (-x)             (y) (-x)
    //
protected:
    int Dim = 1;
    std::vector<int> L; // 不再初始化默认值
    long Vol = 10;
    int NNb = 2;
    int* x_ = nullptr;

public:
    void set(int _Dim, std::initializer_list<int> initList);
    
    long getVol();
    long getNBond();
    int getNNb();
    int getDim();
    int getOpsDir(int _Dir);
    int getComponent(long _Site, int _Dir);
    long getSite(int *_Component);
    long getSite(std::vector<int> &_Component);
    int getDir(long _Site, long _NNSite);
    long getNNSite(long _Site, int _Dir);
    long getNNBond(long _Site, int _Dir);
    int getParity(long _Site);
    long getDirSite(long _Site, int _Dir, int _Length);
};
