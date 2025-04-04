#include "system/Header.hpp"

using namespace std;

int main()
{
    //--------------------------------------------------------//
    //---------- Input-Output System Initialization ----------//
    //--------------------------------------------------------//
    Clock ck;
    IOControl io;

    //--------------------------------------------------------//
    //------------------- Read Input File --------------------//
    //--------------------------------------------------------//
    Parameter para;
    Inputor inp(io, para);

    //--------------------------------------------------------//
    //---------------- Monte Carlo Simulation ----------------//
    //--------------------------------------------------------//
    RandomNumGen rn(para.Seed);
    Observable obs(ck, io, para);
    Histogram his(ck, io, para);
    Configuration cnf(ck, io, rn, para, obs, his);

    unsigned long iter, ieach;

    io.exportInfo(io.OuputInfo, cnf.infoConfig());
    //--------------------------------------------------------//
    //-------------------- Thermalization --------------------//
    //--------------------------------------------------------//
    io.exportInfo(io.OuputInfo, ck.exportNow() + "\tThermalizing...\n");

    obs.reset();
    iter = 0;
	while(true)
	{
		if(iter >= para.N_Therm) break;
        
		// Monte Carlo Sweep
		for(ieach = 0; ieach < para.N_Each; ieach++)
		{
            // Update 
            cnf.updateCnf();
            obs.MCStep += 1e0;

            // Measure
            cnf.measure();
            obs.collectData();
		}
        cnf.checkCnf();

        // Observable Output
        io.exportInfo(io.OuputCont, obs.printOutput(false));

		iter++;
	}

    //--------------------------------------------------------//
    //---------------------- Simulation ----------------------//
    //--------------------------------------------------------//
    io.exportInfo(io.OuputInfo, ck.exportNow() + "\tSimulating...\n");

    obs.reset();
    iter = 0;
	while(true)
	{
		if(iter >= para.N_Total) break;

		// Monte Carlo Sweep
		for(ieach = 0; ieach < para.N_Each; ieach++)
		{
            // Update
            cnf.updateCnf();
            obs.MCStep += 1e0;

            // Measure
            cnf.measure();
            obs.collectData();
		}
        cnf.checkCnf();

        // Observable Output
        io.exportInfo(io.OuputCont, obs.printOutput(true));
        
        // Histogram Output
        his.saveDistr();

        // Current Configuration Output
        cnf.printConfig(iter);

		iter++;
	}

    //--------------------------------------------------------//
    //------------------------- End --------------------------//
    //--------------------------------------------------------//
}