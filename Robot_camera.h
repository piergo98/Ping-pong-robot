#include    <stdlib.h>
#include    <stdio.h>
#include    <math.h>
#include    <allegro.h>
#include    <pthread.h>
#include    <time.h>

#include    "PTask.h"

#define     TARGET_COLOR    0xfa6717    //colore pallina
#define     THRESHOLD       10          //numero minimo di pixel per riconoscere la pallina
#define     DIM             3           //dimensioni buffer per memorizzare i valori passati del centro della pallina
#define     BEFORE          0           //centro precedente
#define     NOW             1           //centro attuale
#define     NEXT            2           //centro successivo
#define     DELTA_X         40          //incremento delle dimensioni della finestra di ricerca lungo x
#define     DELTA_Z         30          //incremento delle dimensioni della finestra di ricerca lungo x
#define     SIZE_X          200         //lunghezza finestra di ricerca
#define     SIZE_Z          200         //altezza finestra di ricerca


struct win{        /* struttura che definisce la finestra di ricerca dell'oggetto */

    int x0;
    int z0;
    int xsize;
    int zsize;

};

struct coord{      /* struttura che contiene le coordinate di un punto */

    int x;
    int z;
};

struct coord buffer[DIM];

struct win   window;       //finestra di ricerca

void init_camera(void);

int centroid(struct win w, struct  coord   *target);    /* Calcola il centro di un certo target, specificato dal colore */

void prediction(struct win *w, struct coord memory[DIM]);    /* Predice il centro successivo e aggiorna la finestra di ricerca */

void* camera(void* arg);          /* Task camera */