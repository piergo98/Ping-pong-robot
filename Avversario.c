#include "Avversario.h"

int col = 14;       // yellow color

void* adversarytask_x(void* arg)
{
    int   i, T;                                         // task index      
    int   xd, vd;                                       // desired position and speed
    int   x,  v;                                        // actual  position and speed
    float   u1[DIM_B],      y,   err[DIM_B],    u;      // temporary variables
    struct  state   temp;                               //temporary structure
        
        i = get_task_index(arg);
        set_activation(i);        

        err[NOW] = err[BEFORE] = 0;
        u1[NOW] = u1[BEFORE]  = 0;

        while(!end) {
        
            if (start  || player) {
                vd = 0;

                pthread_mutex_lock(&s8);
                temp.position = adversary_x.position;
                temp.speed = adversary_x.speed;
                pthread_mutex_unlock(&s8);

                pthread_mutex_lock(&s12);
                if (home){

                    xd  = HALF_X;

                }
                else {
                    pthread_mutex_lock(&s3);
                    xd = buffer[NEXT].x;
                    pthread_mutex_unlock(&s3);
                }
                pthread_mutex_unlock(&s12);

                get_state(&x, &v, &temp);
                //errore di posizione
                err[NOW] = xd-x;
                //controllo di posizione
                u1[NOW] = (KP + Ts * KI) * err[NOW] - KP * err[BEFORE] + u1[BEFORE];
                //controllo di velocita'
                u = u1[NOW] + KD * (vd - v);

                y = motor(u, &adv_x_angle);
                
                pthread_mutex_lock(&s10);
                update_adversary_state_x(y, T, &temp);
                pthread_mutex_unlock(&s10);

                u1[BEFORE] = u1[NOW];
                err[BEFORE] = err[NOW];

                pthread_mutex_lock(&s8);
                adversary_x.position = temp.position;
                adversary_x.speed = temp.speed;
                pthread_mutex_unlock(&s8);
            }

            if (deadline_miss(i))
                show_dmiss(i);

            wait_for_activation(i);
        }
}

void* adversarytask_z(void* arg)
{
    int   i, T, home_tmp;                                   // task index
    int   zd, vd;                                           // desired position and speed
    int   x,  v;                                            // actual  position and speed
    float   u1[DIM_B],      y,   err[DIM_B],    u;          // temporary variables
    struct  state   temp;                                   //temporary structure
        
        i = get_task_index(arg);
        set_activation(i);

        err[NOW] = err[BEFORE] = 0;
        u1[NOW] = u1[BEFORE]  = 0;

        while(!end) {

            if (start || player) {
                
                vd = 0;

                pthread_mutex_lock(&s9);
                temp.position = adversary_z.position;
                temp.speed = adversary_z.speed;
                pthread_mutex_unlock(&s9);

                pthread_mutex_lock(&s12);
                home_tmp = home;
                pthread_mutex_unlock(&s12);

                if (home_tmp){

                    zd  = P4_Z;

                }
                else {

                    pthread_mutex_lock(&s3);
                    zd = buffer[NEXT].z + D;
                    pthread_mutex_unlock(&s3);
                }
                

                get_state(&x, &v, &temp);
                //errore di posizione
                err[NOW] = (zd-x);
                //controllo di posizione
                u1[NOW] = (KP + Ts * KI) * err[NOW] - KP * err[BEFORE] + u1[BEFORE];
                //controllo di velocita'
                u = u1[NOW] + KD * (vd - v);

                y = motor(u, &adv_z_angle);
                
                pthread_mutex_lock(&s10);
                update_adversary_state_z(y, T, &temp);
                pthread_mutex_unlock(&s10);

                u1[BEFORE] = u1[NOW];
                err[BEFORE] = err[NOW];

                pthread_mutex_lock(&s9);
                adversary_z.position = temp.position;
                adversary_z.speed = temp.speed;
                pthread_mutex_unlock(&s9);
            }

            if (deadline_miss(i))
                show_dmiss(i);
                
            wait_for_activation(i);
        }
        
}

void update_adversary_state_x(float y, int T, struct state *robot_tmp)
{
  
    int delta;

        if(!player){

            delta = (int)y * R;                              //converte rotazione del motore in movimento lineare
        
            if (delta > X_MAX ){
                robot_tmp->position = X_MAX;
                robot_tmp->speed = 0;
            }
            else if (delta < X_MIN){
                robot_tmp->position = X_MIN;
                robot_tmp->speed = 0;
            }
            else{
                robot_tmp->speed = (delta - robot_tmp->position)/ Ts;         //rapp. incrementale
                robot_tmp->position = delta;
            }
        }
        else { 
            
            robot_tmp->speed = (mouse_x - robot_tmp->position)/ Ts;
            robot_tmp->position = mouse_x;
        }
}

void update_adversary_state_z(float y, int T, struct state *robot_tmp)
{
  
    int delta;

    if (!player){

        delta = y * R;                              //converte rotazione del motore in movimento cinghia
    
        if (delta > Z_MAX + OFFSET_Z ){
            robot_tmp->position = Z_MAX + OFFSET_Z;
            robot_tmp->speed = 0;
        }
        else if (delta < X_MIN + OFFSET_Z){
            robot_tmp->position = X_MIN + OFFSET_Z;
            robot_tmp->speed = 0;
        }
        else{
            robot_tmp->speed = (delta - robot_tmp->position)/ Ts;         //rapp. incrementale
            robot_tmp->position = delta;
        } 
    }
    else { 
        
        robot_tmp->speed = (mouse_y - robot_tmp->position)/ Ts;
        robot_tmp->position = mouse_y;
    }        
}



