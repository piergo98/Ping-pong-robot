#include "taskmotor.h" 

void* motortask_x(void* arg)
{
    int   i, T;                     // task index
    int   x_min, x_max;             // limiti di movimento      
    int   xd, vd;                   // desired position and speed
    int   x,  v;                    // actual  position and speed
    float   u, z, y;                // temporary variables
        
        i = get_task_index(arg);
        T = tp[i].deadline;         //la utilizzo per il rapp. inc.

        x_min = X_MIN;
        x_max = X_MAX;
        
        prevtheta.in[NOW] = 0;      //sarebbe stato piu' bello un for?
        prevtheta.in[BEFORE] = 0;
        prevtheta.out[NOW] = 0;
        prevtheta.out[BEFORE] = 0;

        while(1) {

            vd = 0;
            xd = buffer[NOW].x;
            get_state(&x, &v, &robot_x);
            u = KP*(xd - x) + KD*(vd - v);
            z = delay(u);
            y = motor(z);
            update_state(y, T, x_min, x_max, &robot_x);
            wait_for_activation(i);
        }
}

void* motortask_z(void* arg)
{
    int   i, T;         // task index
    int   z_min, z_max;             // limiti di movimento
    int   xd, vd;       // desired position and speed
    int   x,  v;        // actual  position and speed
    float   u, z, y;    // temporary variables
        
        i = get_task_index(arg);
        T = tp[i].deadline;       //la utilizzo per il rapp. inc.

        z_min = Z_MIN;
        z_max = Z_MAX;

        prevtheta.in[NOW] = 0;
        prevtheta.in[BEFORE] = 0;
        prevtheta.out[NOW] = 0;
        prevtheta.out[BEFORE] = 0;

        while(!end) {

            vd = 0;
            xd = buffer[NEXT].y;
            get_state(&x, &v, &robot_z);
            u = KP*(xd - x) + KD*(vd - v);
            z = delay(u);
            y = motor(z);
            update_state(y, T, z_min, z_max, &robot_z);
            wait_for_activation(i);
        }
}

float motor(float k)
{
    float theta;
        
        prevtheta.in[NOW] = k;
    
        theta = A * prevtheta.in[NOW] + B * prevtheta.in[BEFORE] + (1 + P) * prevtheta.out[NOW] - P * prevtheta.out[BEFORE];       //transfer function
    
        prevtheta.in[BEFORE] = prevtheta.in[NOW];
        prevtheta.out[BEFORE] = prevtheta.out[NOW];
        prevtheta.out[NOW] = theta;
    
    return theta;
}

void update_state(float y, int T, int p_min, int p_max, struct state *robot_tmp)
{
    y = robot_tmp->position + y * R;                              //converte rotazione del motore in movimento cinghia
    
    robot_tmp->speed = ((int)y - robot_tmp->position)/ T;         //rapp. incrementale
    
    if (y > p_max) 
        robot_tmp->position = p_max;
    
    else if (y < p_min) 
        robot_tmp->position = p_min;
    
    else   
        robot_tmp->position = (int)y;
    
    return;
}

void get_state(float *xi, float *vi, struct state *robot_tmp)
{
    *xi = robot_tmp->position;
    *vi = robot_tmp->speed;

    return;
}