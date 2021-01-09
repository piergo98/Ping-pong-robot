#include<stdio.h>
#include<pthread.h>

#include "PTask.h"
#include "Robot_camera.h" 

#define X_MAX 1  //limiti posizione
#define X_MIN -1
#define Y_MAX 1  //limiti posizione
#define Y_MIN -1

/* parametri funzione di trasferimento: tau = 0.19s, T = 20 ms, K = 19.23 */

#define P 0.19
#define A 0.019
#define B 0.019

#define KP 0.5     //costanti proporzionali e derivative del PID 
#define KD 0.5

#define R 5     //raggio puleggia in mm

struct m_tfunc     //va inizializzata nel motortask (?)                                   
{
   float in[2];    //ingressi instanti k-1, k-2
   float out[2];   //uscite instanti k-1, k-2
} prevtheta;

struct state       // va messa a zero quando accendo 
{
    int position;
    int speed;
};

struct state robot_x;
struct state robot_y;

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

        while(!end) {

            vd = 0;
            xd = buffer[NOW].x;
            get_state(&x, &v, &robot_x);
            u = KP*(xd - x) + KD*(vd - v);
            z = delay(u);
            y = motor(z);
            update_state(y, T, &robot_x);
            wait_for_activation(i);
        }
}

void* motortask_y(void* arg)
{
    int   i, T;         // task index
    int   y_min, y_max;             // limiti di movimento
    int   xd, vd;       // desired position and speed
    int   x,  v;        // actual  position and speed
    float   u, z, y;    // temporary variables
        
        i = get_task_index(arg);
        T = tp[i].deadline;       //la utilizzo per il rapp. inc.
        y_min = Y_MIN;
        y_max = Y_MAX;

        while(!end) {

            vd = 0;
            xd = buffer[NOW].y;
            get_state(&x, &v, &robot_y);
            u = KP*(xd - x) + KD*(vd - v);
            z = delay(u);
            y = motor(z);
            update_state(y, T, &robot_y);
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

void update_state(float y, int T, struct state *robot_tmp)
{
    y = robot_tmp->position + y * R;                              //converte rotazione del motore in movimento cinghia
    
    robot_tmp->speed = ((int)y - robot_tmp->position)/ T;         //rapp. incrementale
    
    if (y > P_MAX) 
        robot_tmp->position = P_MAX;
    
    else if (y < P_MIN) 
        robot_tmp->position = P_MIN;
    
    else   
        robot_tmp->position = (int)y;
    
    return;
}

void get_state(float *xi,float *vi, struct state *robot_tmp)
{
    *xi = robot_tmp->position;
    *vi = robot_tmp->speed;

    return;
}