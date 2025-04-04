#include "../Configuration.hpp"

/* ------------------------------------------------------------------------------------*/
/* ---------------------------------- Spin Operation ----------------------------------*/
/* ------------------------------------------------------------------------------------*/

void Configuration::randSpin_()
{
    switch (Nspin)
    {
        case 1:
        {
            tempSpin[0] = 2 * rn.getRandomNum(2) - 1;
            break;
        }
        case 2:
        {    
            double theta = rn.getRandomDouble(0, 2 * M_PI);
            tempSpin[0] = cos(theta);
            tempSpin[1] = sin(theta);
            break;
        }
        case 3:
        {
            double z = 2 * rn.getRandomDouble() - 1;
            double theta = rn.getRandomDouble(0, 2 * M_PI);
            tempSpin[0] = cos(theta) * sqrt(1 - z * z);
            tempSpin[1] = sin(theta) * sqrt(1 - z * z);
            tempSpin[2] = z;
            break;
        }
        default:
            break;
    }
}

void Configuration::randSpin()
{
    const double epsilon = 1e-16;
    double lengthSquared;
    
    do {
        lengthSquared = 0.0;
        for (int i_N = 0; i_N < Nspin; ++i_N) {
            tempSpin[i_N] = rn.getGaussian(); 
            lengthSquared += tempSpin[i_N] * tempSpin[i_N]; 
        }
    } while (lengthSquared < epsilon); 

    const double length = std::sqrt(lengthSquared);

    for (int i_N = 0; i_N < Nspin; ++i_N) {
        tempSpin[i_N] /= length;
    }
}


void Configuration::flipSpin(long _site, std::vector<double> _axis)
{
    Site[_site] = Site[_site] - 2.0 * Site[_site] * _axis * _axis;
}
