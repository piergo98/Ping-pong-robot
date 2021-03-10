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


#define P 0.769     // parametri funzione di trasferimento:
#define A 0.116     // tau = 0.19s, T = 50 ms, K = 19.23 
#define B 0.106
#define Ts 0.005           //tempo campionamento motore

#define KP 0.5        //costanti proporzionali, derivative e integrative del PID 
#define KD 0.008
#define KI 0.01

#define R 0.5       //raggio puleggia in mm
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
};

struct state       // va messa a zero quando accendo il sistema 
{
    int position;
    int speed;
};

struct status {             // ball structure
    
    int     c;                // color [1,15]
    float   r;                // radius (m)
    float   x;                // x coordinate (m)
    float   y;
    float   z;                // z coordinate (m)
    float   vx;               // x velocity (m/s)
    float   vz;               // z velocity (m/s)
    float   vy;

};

struct m_tfunc rob_x_angle, rob_z_angle, adv_x_angle, adv_z_angle; //angoli di rotazione dei motori

struct state robot_x;           //gestiti dal taskmotor
struct state robot_z;
struct state adversary_x;       //gestiti dal task adversary 
struct state adversary_z;

struct status ball;

int player;                     //flag per la scelta dell'avversario
int mouse_x_flag, mouse_z_flag;

float motor(float k, struct m_tfunc *prevtheta);

void update_state(float y, int T, int p_min, int p_max, struct state *robot_tmp);

void get_state(int *xi, int *vi, struct state *robot_tmp);

void* motortask_x(void* arg);

void* motortask_z(void* arg);

void init_motor(void);
