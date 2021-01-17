#include "Avversario.h"

int col = 14;       // yellow color

void* adversarytask_x(void* arg)
{
    int   i, T;                     // task index
    int   x_min, x_max;             // limiti di movimento      
    int   xd, vd;                   // desired position and speed
    int   x,  v;                    // actual  position and speed
    float   u, z, y;                // temporary variables
        
        i = get_task_index(arg);
        set_activation(i);
        T = tp[i].deadline;         //la utilizzo per il rapp. inc.

        x_min = C_X1 - OFFSET_X;
        x_max = C_X4 + OFFSET_X;
        
        prevtheta.in[NOW] = 0;      //sarebbe stato piu' bello un for?
        prevtheta.in[BEFORE] = 0;
        prevtheta.out[NOW] = 0;
        prevtheta.out[BEFORE] = 0;

        while(!end) {

            vd = 0;
            xd = buffer[NOW].x;
            get_state(&x, &v, &adversary_x);
            u = KP*(xd - x) + KD*(vd - v);
            //z = delay(u);
            y = motor(z);
            update_state(y, T, x_min, x_max, &adversary_x);
            wait_for_activation(i);
        }
}

void* adversarytask_z(void* arg)
{
    int   i, T;         // task index
    int   z_min, z_max;             // limiti di movimento
    int   xd, vd;       // desired position and speed
    int   x,  v;        // actual  position and speed
    float   u, z, y;    // temporary variables
        
        i = get_task_index(arg);
        set_activation(i);
        T = tp[i].deadline;       //la utilizzo per il rapp. inc.

        z_min = C_Z1 - OFFSET_Z;
        z_max = C_Z1 + OFFSET_Z / 3;

        prevtheta.in[NOW] = 0;
        prevtheta.in[BEFORE] = 0;
        prevtheta.out[NOW] = 0;
        prevtheta.out[BEFORE] = 0;

        while(!end) {

            vd = 0;
            xd = buffer[NEXT].z;
            get_state(&x, &v, &adversary_z);
            u = KP*(xd - x) + KD*(vd - v);
            //z = delay(u);
            y = motor(z);
            update_state(y, T, z_min, z_max, &adversary_z);
            wait_for_activation(i);
        }
}