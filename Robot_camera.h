#include    <stdlib.h>
#include    <stdio.h>
#include    <math.h>
#include    <allegro.h>
#include    <pthread.h>
#include    <time.h>

#include    "PTask.h"

#define     TARGET_COLOR    0xFFC060    //colore pallina
#define     THRESHOLD       10          //numero minimo di pixel per riconoscere la pallina
#define     DIM             3           //dimensioni buffer per memorizzare i valori passati del centro della pallina
#define     BEFORE          0           //centro precedente
#define     NOW             1           //centro attuale
#define     NEXT            2           //centro successivo
#define     INCREASE        50          //incremento delle dimensioni della finestra di ricesca
#define     SIZE_X          100         //lunghezza finestra di ricerca
#define     SIZE_Y          100         //altezza finestra di ricerca


struct win{        /* struttura che definisce la finestra di ricerca dell'oggetto */

    int x0;
    int y0;
    int xsize;
    int ysize;

};

struct coord{      /* struttura che contiene le coordinate di un punto */

    int x;
    int y;
};

struct coord buffer[DIM];

struct win*   window;       //finestra di ricerca

int centroid(struct win w, struct  coord   *target);    /* Calcola il centro di un certo target, specificato dal colore */

void prediction(struct win* w);    /* Predice il centro successivo e aggiorna la finestra di ricerca */

void* camera(void* arg);          /* Task camera */