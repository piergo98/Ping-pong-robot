#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>

#include "Robot_camera.h" 

#define X_MAX  1    //limiti posizione
#define X_MIN -1
#define Z_MAX  1    //limiti posizione
#define Z_MIN -1

#define OFFSET_X 90     //distanza raggiungibile al di fuori del tavolo
#define OFFSET_Z 180


#define P 0.19     // parametri funzione di trasferimento:
#define A 0.019    // tau = 0.19s, T = 20 ms, K = 19.23 
#define B 0.019

#define KP 1     //costanti proporzionali e derivative del PID 
#define KD 1

#define R 5        //raggio puleggia in mm
//------------------------------------------------------------------------------
// PUNTI TAVOLO VISTO DALL'ALTO
//------------------------------------------------------------------------------
#define     C_X1    160
#define     C_Z1    420
#define     C_X2    480
#define     C_Z2    60
#define     C_X3    160
#define     C_Z3    60
#define     C_X4    480
#define     C_Z4    420

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

struct state robot_x;        //gestiti dal taskmotor
struct state robot_z;
struct state adversary_x;    //gestiti dal task adversary 
struct state adversary_z;

int player; //flag per la scelta dell'avversario
int mouse_x_flag, mouse_z_flag;

float motor(float k);

void update_state(float y, int T, int p_min, int p_max, struct state *robot_tmp);

void get_state(int *xi, int *vi, struct state *robot_tmp);

void* motortask_x(void* arg);

void* motortask_z(void* arg);

void init_motor(void);
