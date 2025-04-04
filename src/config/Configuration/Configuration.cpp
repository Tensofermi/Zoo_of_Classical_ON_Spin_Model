#include "Configuration.hpp"

Configuration::Configuration(Clock& _ck, IOControl& _io, RandomNumGen& _rn, Parameter& _para, Observable& _obs, Histogram& _his) 
: ck(_ck), io(_io), rn(_rn), para(_para), obs(_obs), his(_his)
{
    initialConf();  // for site spin 
    initialAlgo();  // for algorithms
    initialMeas();  // for k-space and cluster measurement
    initialObsr();  // for basic observables
}

void Configuration::initialConf()
{
    //--- Initialize Basic Parameters
    Nspin = para.Nspin;
    Dim = para.D;
    Beta = para.beta;
    h = para.h;
    L = para.L;

    //--- Initialize Lattice
    Latt.set(Dim, L);
    Vol = Latt.getVol();
    NNb = Latt.getNNb();
    Site.resize(Vol, std::vector<double>(Nspin));
    
    //--- Initialize Temporary Spin
    tempSpin.resize(Nspin);

    //--- Initialize field direction
    fieldDir.resize(Nspin, 0);
    fieldDir[0] = 1;

    //--- Initialize Spin Configuration
    for (unsigned int i = 0; i < Vol; i++)
    {
        randSpin();
        Site[i] = tempSpin;
    }

}

void Configuration::initialAlgo()
{
    //--- Initialize Queue and Memory
    Que.resize(Vol);
    Mem.resize(Vol);
    for (int i = 0; i < Vol; i++)
    {
        Que[i] = 0;
        Mem[i] = 0;
    }

    //--- Initialize Probabilities
    P_metro = 0.0;
    P_sw = 0.0;

}

void Configuration::initialMeas()
{
    //--- k-space Measurement
    k_vec.resize(Dim);
    for (int i = 0; i < Dim; i++) 
        k_vec[i] = 0;

    k_vec[0] = 2 * M_PI / L;    // kx = 2 Pi / L

    k_cos.resize(Vol);
    k_sin.resize(Vol);

    std::vector<double> coor;
    coor.resize(Dim);
    for (int i = 0; i < Vol; i++)
    {
        for (int j = 0; j < Dim; j++)
            coor[j] = Latt.getComponent(i, j);

        k_cos[i] = cos(k_vec * coor);
        k_sin[i] = sin(k_vec * coor);
    }
    coor.clear();

    //--- Geometric Measurement
    x_max.resize(Dim);
    x_min.resize(Dim);
    x_now.resize(Dim);

    for (int i = 0; i < Dim; i++)
    {
        x_max[i] = 0;
        x_min[i] = 0;
        x_now[i] = 0;
    }

}

void Configuration::initialObsr()
{
    //--- Initialize Cluster Observables
    NCluster = 0;
    C1 = 0;
    C2 = 0;
    S2 = 0;
    S4 = 0;

    para.Corr_Fun.resize(L);
}

#include "../Measurement_config.hpp"

bool Configuration::measureOrNot()
{
    return true;
}

void Configuration::writeCnf()
{
    
}

void Configuration::checkCnf()
{

}
