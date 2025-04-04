#include "../Configuration.hpp"

/* ------------------------------------------------------------------------------------*/
/* ------------------------------ Short-Range Interaction -----------------------------*/
/* ------------------------------------------------------------------------------------*/

void Configuration::SwendsenWang()
{
    long j_Site, k_Site;
    double project;
    int rear, front;
    double before;
    double after;

    //--- Clear Observables
    NCluster = 0;
    C1 = 0;
    C2 = 0;
    S2 = 0;
    S4 = 0;

    //--- Clear Queue and Memory
    for (unsigned int i = 0; i < Vol; i++)
    {
        Que[i] = 0;
        Mem[i] = 0;
    }

    //--- Randomly choose an axis to project
    randSpin();

    //--- BFS Search
    for (long i_Site = 0; i_Site < Vol; i_Site++)
    {
        //--- Check Whether Visited
        if(Mem[i_Site] == 1) continue;
        Mem[i_Site] = 1;

        NCluster++;

        rear = 0;
        front = 1;
        Que[front] = i_Site;     // start from 1 !

        while(front > rear)
        {
            rear++;
            j_Site = Que[rear];
            for (int j = 0; j < NNb; j++)
            {
                // obtain the interacting k_Site
                k_Site = Latt.getNNSite(j_Site, j);
                project = (tempSpin * Site[j_Site]) * (tempSpin * Site[k_Site]);
                P_sw = 1.0 - exp(-2 * Beta * project);
                if(Mem[k_Site] == 0 && rn.getRandomDouble() < P_sw)
                {
                    front++;
                    Mem[k_Site] = 1;
                    Que[front] = k_Site;
                }
            }
        }

        //--- Cluster Measurement
        S2 = S2 + pow(front, 2);
        S4 = S4 + pow(front, 4);

        if(front > C1)
        {
            C2 = C1;
            C1 = front;
        }
        else if(front > C2)
        {
            C2 = front;
        }

        // his.obsAdd(para.his_ns, front);

        if(h!=0)
        {
            before = 0.0;
            after = 0.0;
            //--- Calculate Field Energy
            for (int j = 1; j <= front; j++)    // start from 1 !
            {
                before -= h * Site[Que[j]][0];
                after -= h * (Site[Que[j]] - 2.0 * Site[Que[j]] * tempSpin * tempSpin)[0];
            }
    
            //--- Flip Cluter Spins
            if(rn.getRandomDouble() < exp(- Beta * after) / (exp(- Beta * after) + exp(- Beta * before)))
            {
                for (int j = 1; j <= front; j++)    // start from 1 !
                    flipSpin(Que[j], tempSpin);
            }
        }
        else    // no magnetic field case
        {
            //--- Flip Cluter Spins
            if(rn.getRandomDouble() < 0.5)
            {
                for (int j = 1; j <= front; j++)    // start from 1 !
                    flipSpin(Que[j], tempSpin);
            }
        }

    }

}