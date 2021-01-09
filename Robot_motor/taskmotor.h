#include    <stdio.h>
#include    <stdlib.h>
#include    <allegro.h>
#include    <pthread.h>

#include    "PTask.h"
#include    "Robot_camera.h" 

#define     X_MAX   1  //limiti posizione
#define     X_MIN   -1
#define     Y_MAX   1  //limiti posizione
#define     Y_MIN   -1

/* parametri funzione di trasferimento: tau = 0.19s, T = 20 ms, K = 19.23 */

#define     P   0.19
#define     A   0.019
#define     B   0.019

#define     KP  0.5     //costanti proporzionali e derivative del PID 
#define     KD  0.5

#define     R   5       //raggio puleggia in mm

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

void* motortask_x(void* arg);

void* motortask_y(void* arg);

float motor(float k);

void update_state(float y, int T, struct state *robot_tmp, int max, int min);

void get_state(float *xi,float *vi, struct state *robot_tmp);