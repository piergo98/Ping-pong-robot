#include    <stdlib.h>
#include    <stdio.h>
#include    <math.h>
#include    <allegro.h>
#include    <pthread.h>
#include    <time.h>

#include    "PTask.h"
#include    "taskmotor.h"
#include    "Robot_camera.h"

#define G0           9.8    // acceleration of gravity
#define TLEN         30     // trail length
#define HMIN         200    // min initial height
#define HMAX         390    // max initial height
#define VXMIN        20     // min initial hor. speed
#define VXMAX        10     // max initial hor. speed
#define DUMP         0.9    // dumping coefficient
#define TSCALE       10     // time scale factor
#define BETA         0.2    // adimensional braking coefficient
#define ERR_MIN      0
#define ERR_MAX      1

/* guardando il PO vediamo solo le coordinate x e z */

struct status {             // ball structure
    
    int c;                  // color [1,15]
    float r;                // radius (m)
    float x;                // x coordinate (m)
    float y;
    float z;                // z coordinate (m)
    float vx;               // x velocity (m/s)
    float vz;               // z velocity (m/s)
    float vy;
    //float v0;               // jumping velocity (m/s)

};

struct cbuf {               // circular buffer structure
    
    int top;                // index of the current element
    int x[TLEN];            // array of x coordinates
    int z[TLEN];            // array of z coordinates
    int y[TLEN];
    
};

struct cbuf trail;          
struct status ball;

void store_trail( int i);

void handle_bounce(int i);

float frand(float vxmin, float vxmax);