#pragma once
#include <bits/stdc++.h>

class Hypercubic
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
    int L = 10;
    long Vol = 10;
    int NNb = 2;

    int* x_ = nullptr;

public:
    void set(int _Dim, int _L);
    
    int getL();                                                // Get L 
    long getVol();                                             // Get NSite or Vol
    long getNBond();                                           // Get NBond
    int getNNb();                                              // Get NNb
    int getDim();                                              // Get Dim
    int getOpsDir(int _Dir);                                   // Get the opposite direction of _Dir

    int getComponent(long _Site, int _Dir);                    // Get component in direction _Dir of site _Site
    long getSite(int *_Component);                             // Get the site for component _Component
    long getSite(std::vector<int> &_Component);                // Get the site for component _Component [vector version]

    int getDir(long _Site, long _NNSite);                      // Get Direction
    long getNNSite(long _Site, int _Dir);                      // Get the nearest neighbor site in the direction _Dir
    long getNNBond(long _Site, int _Dir);                      // Get the nearest neighbor bond in the direction _Dir
    int getParity(long _Site);                                 // Get Parity
    long getDirSite(long _Site, int _Dir, int _Length);
    
};
