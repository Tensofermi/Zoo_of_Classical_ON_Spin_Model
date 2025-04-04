#include "../Configuration.hpp"

/* ------------------------------------------------------------------------------------*/
/* ------------------------------ Short-Range Interaction -----------------------------*/
/* ------------------------------------------------------------------------------------*/

void Configuration::Metropolis()
{
    long i_Site, j_Site;
    double dE;
    std::vector<double> dV;

    for (unsigned int i = 0; i < Vol; i++)
    {
        // i_Site = rn.getRandomNum(Vol);
        i_Site = i;
        
        randSpin();
        
        dV = tempSpin - Site[i_Site];

        //--- Calculate Delta Energy
        dE = 0.0;
        for (int j = 0; j < NNb; j++)
        {
            j_Site = Latt.getNNSite(i_Site, j);
            dE = dE - dV * Site[j_Site];
        }
        dE += - h * dV[0];

        //--- Attempt to Update
        P_metro = exp(- Beta * dE);
        if(rn.getRandomDouble() < P_metro)
            Site[i_Site] = tempSpin;
    }

}
