#include    <allegro.h>

#define     COLOR   32              //profondita di colori: 16 bit
#define     VERTEX  8
#define     P_X     20
#define     P_Z     20
#define     X_LEG   490
#define     FIELD   0x9acd32        //colore del campo in esadecimale
#define     WHITE   0xffffff        //bianco in esadecimale
#define     BLACK   0x000000        //nero in esadecimale
#define     RED     0xff0000        //rosso in esadecimale
#define     BLUE    0x0000ff        //blu in esadecimale
#define     TRASP   -1
#define     WIDTH   640
#define     HEIGTH  480

int tempo, pos_old, vel_old;

void init_screen(void);  /* Inizializza lo schermo e crea il task grafico */

void draw_graph(BITMAP* buf);          /* Disegna l'interfaccia su una bitmap */

void *display(void* arg);       /* Task grafico */

void* command(void* arg);          // aggiorna i flag in base ai tasti premuti