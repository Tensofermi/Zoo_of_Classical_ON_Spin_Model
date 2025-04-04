#include "../Configuration.hpp"

/* ------------------------------------------------------------------------------------*/
/* ------------------------------ Short-Range Interaction -----------------------------*/
/* ------------------------------------------------------------------------------------*/

void Configuration::Wolff()
{
    long i_Site, j_Site;
    double project;
    int rear, front;
    double before;
    double after;

    //--- Clear Queue and Memory
    for (unsigned int i = 0; i < Vol; i++)
    {
        Que[i] = 0;
        Mem[i] = 0;
    }

    //--- Randomly choose an axis to project
    randSpin();
    
    //--- BFS Search
    i_Site = rn.getRandomNum(Vol);
    Mem[i_Site] = 1;

    rear = 0; 
    front = 1;
    Que[front] = i_Site;    // start from 1 !

    while(front > rear)
    {
        rear++;
        i_Site = Que[rear];
        for (int i = 0; i < NNb; i++)
        {
            j_Site = Latt.getNNSite(i_Site, i);
            project = (tempSpin * Site[i_Site]) * (tempSpin * Site[j_Site]);
            P_sw = 1.0 - exp(-2 * Beta * project);
            if(Mem[j_Site] == 0 && rn.getRandomDouble() < P_sw)
            {
                front++; 
                Mem[j_Site] = 1;
                Que[front] = j_Site; 
            }
        }
    }

    if(h!=0)
    {
        //--- Calculate Field Energy
        before = 0.0;
        after = 0.0;
        for (int j = 1; j <= front; j++)    // start from 1 !
        {
            before -= h * Site[Que[j]][0];
            after -= h * (Site[Que[j]] - 2.0 * Site[Que[j]] * tempSpin * tempSpin)[0];
        }

        //--- Flip Cluter Spins
        if(rn.getRandomDouble() < exp(- Beta * after) / exp(- Beta * before))
        {
            for (int j = 1; j <= front; j++)    // start from 1 !
                flipSpin(Que[j], tempSpin);
        }
    }
    else    // no magnetic field case
    {
        //--- Flip Cluter Spins
        for (int j = 1; j <= front; j++)    // start from 1 !
            flipSpin(Que[j], tempSpin);

    }

}
