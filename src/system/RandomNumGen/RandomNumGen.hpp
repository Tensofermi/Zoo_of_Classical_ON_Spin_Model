#pragma once
#include "common.h"
 
class RandomNumGen
{
    double tm32;
    int mod2, mul2;
    int len1, ifd1;
    int len2, ifd2;
    std::vector<int> inxt1;
    std::vector<int> inxt2;
    std::vector<std::uint32_t> ir1;
    std::vector<std::uint32_t> ir2;
    int ipnt1, ipnf1;
    int ipnt2, ipnf2;
    int mxrn;
    std::vector<std::uint32_t> irn;
    int nrannr;

    std::uint32_t nextWord();
    
public:
    RandomNumGen();
    RandomNumGen(int Seed);
    void initRandomNumGen(int Seed);                // initialize 
    std::int32_t getRandomNum();                    // output -2^31 ~ 2^31-1
    int getRandomNum(int MaxNum);                   // output 0 ~ MaxNum-1
    long getRandomNum(long MaxNum);                 // output 0 ~ MaxNum-1
    double getRandomDouble();                       // output [0,1)
    double getRandomDouble(double a, double b);     // output [a,b)
    double getGaussian();                           // output Gaussian Distribution
};
