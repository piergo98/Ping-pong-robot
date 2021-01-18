#include    <stdlib.h>
#include    <stdio.h>
#include    <math.h>
#include    <allegro.h>
#include    <pthread.h>
#include    <time.h>

#include    "taskmotor.h"

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


void* adversarytask_x(void* arg);

void* adversarytask_z(void* arg);
