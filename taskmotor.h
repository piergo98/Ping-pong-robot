#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>

//#include "PTask.h"
#include "Robot_camera.h" 

#define X_MAX 1    //limiti posizione
#define X_MIN -1
#define Z_MAX 1    //limiti posizione
#define Z_MIN -1


#define P 0.19     // parametri funzione di trasferimento:
#define A 0.019    // tau = 0.19s, T = 20 ms, K = 19.23 
#define B 0.019

#define KP 0.5     //costanti proporzionali e derivative del PID 
#define KD 0.5

#define R 5        //raggio puleggia in mm

struct m_tfunc     //va inizializzata nel motortask (?)                                   
{
   float in[2];    //ingressi instanti k-1, k-2
   float out[2];   //uscite instanti k-1, k-2
} prevtheta;

struct state       // va messa a zero quando accendo il sistema 
{
    int position;
    int speed;
};

struct state robot_x;        //getsiti del taskmotor
struct state robot_z;
struct state adversary_x;    //gestiti dal task adversary 
struct state adversary_z;

float motor(float k);

void update_state(float y, int T, int p_min, int p_max, struct state *robot_tmp);

void get_state(int *xi, int *vi, struct state *robot_tmp);