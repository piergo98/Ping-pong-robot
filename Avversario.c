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
        //sem_wait(&s1);
        T = tp[i].deadline;         //la utilizzo per il rapp. inc.
        //sem_post(&s1);

        x_min = C_X1 - OFFSET_X;
        x_max = C_X4 + OFFSET_X;

        sem_wait(&s5); 
        prevtheta.in[NOW] = 0;      //sarebbe stato piu' bello un for?
        prevtheta.in[BEFORE] = 0;
        prevtheta.out[NOW] = 0;
        prevtheta.out[BEFORE] = 0;
        sem_post(&s5);

        //sem_wait(&s2);
        while(!end) {

            vd = 0;

            sem_wait(&s3);
            xd = buffer[NOW].x;
            sem_post(&s3);

            sem_wait(&s8);
            get_state(&x, &v, &adversary_x);
            sem_post(&s8);
            u = KP*(xd - x) + KD*(vd - v);
            //z = delay(u);
            y = motor(z);
            
            sem_wait(&s10);
            if (player){
                sem_wait(&s11);
                mouse_x_flag = 1;
                sem_post(&s11);
            }
            sem_post(&s10);
            sem_wait(&s8);
            update_state(y, T, x_min, x_max, &adversary_x);
            sem_post(&s8);
            sem_wait(&s11);
            mouse_x_flag = 0;
            sem_post(&s11);
            

            if (deadline_miss(i))
                show_dmiss(i);

            wait_for_activation(i);
        }
        //sem_post(&s2);
}

void* adversarytask_z(void* arg)
{
    int   i, T;         // task index
    int   z_min, z_max;             // limiti di movimento
    int   zd, vd;       // desired position and speed
    int   x,  v;        // actual  position and speed
    float   u, z, y;    // temporary variables
        
        i = get_task_index(arg);
        set_activation(i);
        //sem_wait(&s1);
        T = tp[i].deadline;       //la utilizzo per il rapp. inc.
        //sem_post(&s1);

        z_min = C_Z1 - OFFSET_Z;
        z_max = C_Z1 + OFFSET_Z / 3;

        sem_wait(&s5);
        prevtheta.in[NOW] = 0;
        prevtheta.in[BEFORE] = 0;
        prevtheta.out[NOW] = 0;
        prevtheta.out[BEFORE] = 0;
        sem_post(&s5);

        //sem_wait(&s2);
        while(!end) {

            vd = 0;

            sem_wait(&s3);
            zd = buffer[NEXT].z;
            sem_post(&s3);

            sem_wait(&s9);
            get_state(&x, &v, &adversary_z);
            u = KP*(zd - x) + KD*(vd - v);
            //z = delay(u);
            y = motor(z);
            sem_wait(&s12);
            sem_wait(&s10);
            if (player)
                mouse_z_flag = 1;
            sem_post(&s10);
            update_state(y, T, z_min, z_max, &adversary_z);
            mouse_z_flag = 0;
            sem_post(&s12);
            sem_post(&s9);

            if (deadline_miss(i))
                show_dmiss(i);
                
            wait_for_activation(i);
        }
        //sem_post(&s2);
}