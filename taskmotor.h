#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>

#include "Robot_camera.h" 

//------------------------------------------------------------------------------
// LIMITI DI POSIZIONE RACCHETTE LUNGO X E Z
//------------------------------------------------------------------------------
#define X_MAX   570    
#define X_MIN   70
#define Z_MAX   268 
#define Z_MIN   128

#define OFFSET_Z 340    //offset per i limiti di movimento della racchetta avversario
//------------------------------------------------------------------------------
// PARAMETRI FUNZIONE DI TRASFERIMENTO MOTORI CON:
// TAU = 0.19 s      T = 50 ms      K = 19.23
//------------------------------------------------------------------------------
#define P 0.769     
#define A 0.116
#define B 0.106
#define Ts 0.005           //tempo campionamento motore
//------------------------------------------------------------------------------
// COSTANTI PROPORZIONALI, DERIVATIV E INTEGRATIVE DEL CONTROLLORE PID
//------------------------------------------------------------------------------
#define KP 0.5        
#define KD 0.008
#define KI 0.01
//------------------------------------------------------------------------------
// ALTRE COSTANTI
//------------------------------------------------------------------------------
#define R 0.5       //raggio puleggia in mm
#define D 5         //offset usato lungo z sia per motor_z che per adversary_z e ball_z
//------------------------------------------------------------------------------


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
int start;
int home;

float motor(float k, struct m_tfunc *prevtheta);

void update_state_x(float y, float e, struct state *robot_tmp);

void update_state_z(float y, float e, struct state *robot_tmp);

void get_state(int *xi, int *vi, struct state *robot_tmp);

void* motortask_x(void* arg);

void* motortask_z(void* arg);

void init_motor(void);
