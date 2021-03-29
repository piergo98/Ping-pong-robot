#include    <stdlib.h>
#include    <stdio.h>
#include    <math.h>
#include    <allegro.h>
#include    <pthread.h>
#include    <time.h>

#include    "Avversario.h"

//---------------------------------------------------------------------------------------------
// COSTANTI FISICHE
//---------------------------------------------------------------------------------------------
#define     G0              9.8     // acceleration of gravity
#define     HMIN            200     // min initial height
#define     HMAX            390     // max initial height
#define     VXMIN           20      // min initial hor. speed
#define     VXMAX           10      // max initial hor. speed
#define     Y_0             20      // initial heigth
#define     DAMP_X          0.001   // damping coefficient
#define     DAMP_Z          0.0001  // damping coefficient
#define     BETA            0.2     // adimensional braking coefficient
//---------------------------------------------------------------------------------------------
// LIMITI ERRORI CASUALI
//---------------------------------------------------------------------------------------------
#define     ERR_MIN         -7
#define     ERR_MAX         7
//---------------------------------------------------------------------------------------------
// LUNGHEZZA E LARGHEZZA AREA DI GIOCO (640X480)
//---------------------------------------------------------------------------------------------
#define     WIDTH_GAME      640
#define     HEIGHT_GAME     768
#define     DELTA           128     //traslazione lungo z della zona di gioco
//---------------------------------------------------------------------------------------------
// COORDINATE INIZIALI DELLA PALLINA
//---------------------------------------------------------------------------------------------
#define     INIT_X          320
#define     INIT_Z          248
#define     BALL_RADIUS     5
#define     VZ_FACILE       20
#define     VZ_MEDIO        30
#define     VZ_DIFFICILE    40
#define     VZ_ESTREMA      50
//---------------------------------------------------------------------------------------------
// DIMENSIONI RACCHETTA
//---------------------------------------------------------------------------------------------
#define     RACC_MAX    20      // max racchetta
#define     RACC_MIN    20      // min racchetta
//---------------------------------------------------------------------------------------------
// DIMENSIONI RACCHETTA
//---------------------------------------------------------------------------------------------
#define     EPS         30      // offset usato per le condizioni di rimbalzo
//---------------------------------------------------------------------------------------------

int     p_rob;          //punteggio robot
int     p_avv;          //punteggio avversario

void handle_bounce(int i);

float frand(float vxmin, float vxmax);

void init_ball(void);

void* balltask(void* arg);

void chi_gioca();