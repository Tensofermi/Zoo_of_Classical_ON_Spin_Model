#pragma once
#include <bits/stdc++.h>
 
class RandomNumGen
{
    double tm32;
    int mult;
    int mod2, mul2;
    int len1, ifd1;
    int len2, ifd2;
    int* inxt1 = nullptr;
    int* inxt2 = nullptr;
    int* ir1 = nullptr; 
    int* ir2 = nullptr;
    int ipnt1, ipnf1;
    int ipnt2, ipnf2;
    int mxrn;
    int* irn = nullptr;
    
	int Seed;
    int nrannr;
    
public:
    RandomNumGen();
    ~RandomNumGen();
    RandomNumGen(int Seed);
    void initRandomNumGen(int Seed);                // initialize 
    int getRandomNum();                             // output -2^31 ~ 2^31-1
    int getRandomNum(int MaxNum);                   // output 0 ~ MaxNum-1
    long getRandomNum(long MaxNum);                 // output 0 ~ MaxNum-1
    double getRandomDouble();                       // output [0,1)
    double getRandomDouble(double a, double b);     // output [a,b)
    double getGaussian();                           // output Gaussian Distribution
};