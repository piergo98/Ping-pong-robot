#include    <stdlib.h>
#include    <stdio.h>
#include    <allegro.h>

#define     WIDTH   640
#define     HEIGTH  480
#define     COLOR   16              //profondita di colori: 16 bit
#define     VERTEX  8
#define     P_X     20
#define     P_Y     20
#define     FIELD   0x9acd32        //colore del campo in esadecimale
#define     WHITE   0xffffff        //bianco in esadecimale
#define     TRASP   -1
#define     DIM_S   10

char    punti_rob[DIM_S];
char    punti_avv[DIM_S];
extern  int p_rob;          //punteggio robot
extern  int p_avv;          //punteggio avversario

void init_screen(); //inizializza lo schermo