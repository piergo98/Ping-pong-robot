#include "Avversario.h"

int col = 14;       // yellow color

void* adversarytask_x(void* arg)
{
    int   i, T;                     // task index
    int   x_min, x_max;             // limiti di movimento      
    int   xd, vd;                   // desired position and speed
    int   x,  v;                    // actual  position and speed
    float   u1[2],      y,   err[2],    u; // temporary variables
    struct  state   temp;           //temporary structure
        
        i = get_task_index(arg);
        set_activation(i);
        //sem_wait(&s1);
        T = tp[i].deadline/ 10;         //la utilizzo per il rapp. inc.
        //sem_post(&s1);

        x_min = C_X1 - OFFSET_X;
        x_max = C_X4 + OFFSET_X;

        err[NOW] = err[BEFORE] = 0;
        u1[NOW] = u1[BEFORE]  = 0;

        via_adv = 0;

        while(!end) {

            if (!via_adv) {

                vd = 0;

                pthread_mutex_lock(&s8);
                temp.position = adversary_x.position;
                temp.speed = adversary_x.speed;
                pthread_mutex_unlock(&s8);

                pthread_mutex_lock(&s3);
                xd = buffer[NEXT].x;
                pthread_mutex_unlock(&s3);

                get_state(&x, &v, &temp);
                //errore di posizione
                err[NOW] = xd-x;
                //controllo di posizione
                u1[NOW] = (KP + Ts * KI) * err[NOW] - KP * err[BEFORE] + u1[BEFORE];
                //controllo di velocita'
                u = u1[NOW] + KD * (vd - v);
                
                printf("u_adv_x = %f\t", u);
                printf("e_adv_x = %f\n", err[NOW]);

                y = motor(u, &adv_x_angle);
                
                //pthread_mutex_lock(&s10);
                update_adversary_state_x(y, T, x_min, x_max, &temp);
                //pthread_mutex_unlock(&s10);

                u1[BEFORE] = u1[NOW];
                err[BEFORE] = err[NOW];

                pthread_mutex_lock(&s8);
                adversary_x.position = temp.position;
                adversary_x.speed = temp.speed;
                pthread_mutex_unlock(&s8);

                if (err[NOW]<=4 && err[NOW]>= -4) via_adv = 1;
            }

            if (deadline_miss(i))
                show_dmiss(i);

            wait_for_activation(i);
        }
        //sem_post(&s2);
}

void* adversarytask_z(void* arg)
{
    int   i, T;                         // task index
    int   z_min, z_max;                 // limiti di movimento
    int   zd, vd;                       // desired position and speed
    int   x,  v;                        // actual  position and speed
    float   u1[2],      y,   err[2],    u;     // temporary variables
    struct  state   temp;               //temporary structure
        
        i = get_task_index(arg);
        set_activation(i);
        //sem_wait(&s1);
        T = tp[i].period / 10;       //la utilizzo per il rapp. inc.
        //sem_post(&s1);

        z_min = C_Z1 - OFFSET_Z;
        z_max = C_Z1 + OFFSET_Z / 3;

        err[NOW] = err[BEFORE] = 0;
        u1[NOW] = u1[BEFORE]  = 0;

        while(!end) {

            if (via_adv) {

                vd = 0;

                pthread_mutex_lock(&s9);
                temp.position = adversary_z.position;
                temp.speed = adversary_z.speed;
                pthread_mutex_unlock(&s9);

                pthread_mutex_lock(&s3);
                zd =buffer[NEXT].z + 2;
                pthread_mutex_unlock(&s3);

                //pthread_mutex_lock(&s13);
                //zd =ball.z;
                //pthread_mutex_unlock(&s13);

                get_state(&x, &v, &temp);
                //errore di posizione
                err[NOW] = zd-x;
                //controllo di posizione
                u1[NOW] = (KP + Ts * KI) * err[NOW] - KP * err[BEFORE] + u1[BEFORE];
                //controllo di velocita'
                u = u1[NOW] + KD * (vd - v);

                printf("u_adv_z = %f\t", u);
                printf("e_adv_z = %f\n", err[NOW]);

                y = motor(u, &adv_z_angle);

                
                //pthread_mutex_lock(&s10);
                update_adversary_state_z(y, T, z_min, z_max, &temp);
                //pthread_mutex_unlock(&s10);

                u1[BEFORE] = u1[NOW];
                err[BEFORE] = err[NOW];

                pthread_mutex_lock(&s9);
                adversary_z.position = temp.position;
                adversary_z.speed = temp.speed;
                pthread_mutex_unlock(&s9);

                if (err[NOW]<=4 && err[NOW]>= -4) via_adv =0;

            }

            if (deadline_miss(i))
                show_dmiss(i);
                
            wait_for_activation(i);
        }
        
}

void update_adversary_state_x(float y, int T, int p_min, int p_max, struct state *robot_tmp)
{
  
    int pippo;

    pippo = y * R;                              //converte rotazione del motore in movimento cinghia
    
        if (pippo > p_max )
            robot_tmp->position = p_max;

        else if (pippo < p_min)
            robot_tmp->position = p_min;

        else if (player)
            robot_tmp->position = mouse_x;

        else{
            robot_tmp->speed = ((int)pippo - robot_tmp->position)/ T;         //rapp. incrementale
            robot_tmp->position = (int)pippo;
        } 
}

void update_adversary_state_z(float y, int T, int p_min, int p_max, struct state *robot_tmp)
{
  
    int pippo;

    pippo = y * R;                              //converte rotazione del motore in movimento cinghia
    
        if (pippo > p_max )
            robot_tmp->position = p_max;

        else if (pippo < p_min)
            robot_tmp->position = p_min;

        else if (player)
            robot_tmp->position = mouse_y;

        else{
            robot_tmp->speed = ((int)pippo - robot_tmp->position)/ T;         //rapp. incrementale
            robot_tmp->position = (int)pippo;
        } 
}



