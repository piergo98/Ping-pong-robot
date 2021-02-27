#include "Avversario.h"

int col = 14;       // yellow color

void* adversarytask_x(void* arg)
{
    int   i, T;                     // task index
    int   x_min, x_max;             // limiti di movimento      
    int   xd, vd;                   // desired position and speed
    int   x,  v;                    // actual  position and speed
    float   u, z, y;                // temporary variables
    struct  state   temp;           //temporary structure
        
        i = get_task_index(arg);
        set_activation(i);
        //sem_wait(&s1);
        T = tp[i].deadline;         //la utilizzo per il rapp. inc.
        //sem_post(&s1);

        x_min = C_X1 - OFFSET_X;
        x_max = C_X4 + OFFSET_X;

        pthread_mutex_lock(&s5); 
        prevtheta.in[NOW] = 0;      //sarebbe stato piu' bello un for?
        prevtheta.in[BEFORE] = 0;
        prevtheta.out[NOW] = 0;
        prevtheta.out[BEFORE] = 0;
        pthread_mutex_unlock(&s5);

        pthread_mutex_lock(&s8);
        temp.position = adversary_x.position;
        temp.speed = adversary_x.speed;
        pthread_mutex_unlock(&s8);

        //sem_wait(&s2);
        while(!end) {

            vd = 0;

            pthread_mutex_lock(&s3);
            xd = buffer[NEXT].x;
            pthread_mutex_unlock(&s3);

            get_state(&x, &v, &temp);
            u = KP*(xd - x) + KD*(vd - v);
            y = motor(u);
            
            pthread_mutex_lock(&s10);
            if (player){
                pthread_mutex_lock(&s11);
                mouse_x_flag = 1;
                pthread_mutex_unlock(&s11);
            }
            pthread_mutex_unlock(&s10);

            update_state(y, T, x_min, x_max, &temp);
            pthread_mutex_lock(&s11);
            mouse_x_flag = 0;
            pthread_mutex_unlock(&s11);

            pthread_mutex_lock(&s8);
            adversary_x.position = temp.position;
            adversary_x.speed = temp.speed;
            pthread_mutex_unlock(&s8);
            

            if (deadline_miss(i))
                show_dmiss(i);

            wait_for_activation(i);
        }
        //sem_post(&s2);
}

void* adversarytask_z(void* arg)
{
    int   i, T;                 // task index
    int   z_min, z_max;         // limiti di movimento
    int   zd, vd;               // desired position and speed
    int   x,  v;                // actual  position and speed
    float   u, z, y;            // temporary variables
    struct  state   temp;       //temporary structure
        
        i = get_task_index(arg);
        set_activation(i);
        //sem_wait(&s1);
        T = tp[i].deadline;       //la utilizzo per il rapp. inc.
        //sem_post(&s1);

        z_min = C_Z1 - OFFSET_Z;
        z_max = C_Z1 + OFFSET_Z / 3;

        pthread_mutex_lock(&s5);
        prevtheta.in[NOW] = 0;
        prevtheta.in[BEFORE] = 0;
        prevtheta.out[NOW] = 0;
        prevtheta.out[BEFORE] = 0;
        pthread_mutex_unlock(&s5);

        pthread_mutex_lock(&s9);
        temp.position = adversary_z.position;
        temp.speed = adversary_z.speed;
        pthread_mutex_unlock(&s9);

        //sem_wait(&s2);
        while(!end) {

            vd = 0;

            pthread_mutex_lock(&s3);
            zd = buffer[NEXT].z;
            pthread_mutex_unlock(&s3);

            get_state(&x, &v, &temp);
            u = KP*(zd - x) + KD*(vd - v);
            y = motor(u);

            pthread_mutex_lock(&s12);
            pthread_mutex_lock(&s10);
            if (player)
                mouse_z_flag = 1;
            pthread_mutex_unlock(&s10);
            update_state(y, T, z_min, z_max, &temp);
            mouse_z_flag = 0;
            pthread_mutex_unlock(&s12);

            pthread_mutex_lock(&s9);
            adversary_z.position = temp.position;
            adversary_z.speed = temp.speed;
            pthread_mutex_unlock(&s9);

            if (deadline_miss(i))
                show_dmiss(i);
                
            wait_for_activation(i);
        }
        //sem_post(&s2);
}