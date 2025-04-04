#pragma once
#include <bits/stdc++.h>
#include "../../system/Header.hpp"
#include "../Lattice/Lattice.hpp"

class Configuration
{
    Clock& ck;
    IOControl& io;
    RandomNumGen& rn;
    Parameter& para;
    Observable& obs;
    Histogram& his;

public:
    //--- Configuration
    std::vector<std::vector<double>> Site;
    Hypercubic Latt;            
    int Dim, Nspin, L, NNb;
    long Vol;
    double Beta, h;

    //--- Basic parameter for algorithms
    std::vector<int> Mem, Que;
    double P_metro, P_sw;

    //--- For spin flip and k-space
    std::vector<double> tempSpin, fieldDir;
    std::vector<double> k_vec, k_cos, k_sin;

    //--- Basic Observables
    std::vector<int> x_max, x_min, x_now;
    long NCluster;                  // Number of clusters
    double C1, C2, S2, S4;          // Cluster size defined by particle number

public:
std::string infoConfig()
{
    return 
        "====================\n"
        "This program simulates the O(N) spin model.\n"
        "It can simulate the O(N) model in any spatial dimension and with any spin dimension.\n"
        "====================\n";
};


    Configuration(Clock& _ck, IOControl& _io, RandomNumGen& rn, Parameter& _para, Observable& _obs, Histogram& _his);
    void initialConf();
    void initialAlgo();
    void initialMeas();
    void initialObsr();

    void updateCnf();
    bool measureOrNot();
    void measure();
    void writeCnf();

    void printConfig(int _index);
    void squarePrint();
    void corrFunPrint();

    void checkCnf();

    // Basic Operation
    void randSpin();
    void randSpin_();
    void flipSpin(long _site, std::vector<double> _axis);
    
    // Update Algorithm
    void Metropolis();
    void Wolff();
    void SwendsenWang();


};
