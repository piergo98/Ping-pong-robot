#include    <allegro.h>


#define     COLOR   32              //profondita di colori: 16 bit
#define     VERTEX  8
#define     P_X     20
#define     P_Z     20
#define     X_LEG   480
#define     FIELD   0x9acd32        //colore del campo in esadecimale
#define     WHITE   0xffffff        //bianco in esadecimale
#define     BLACK   0x000000        //nero in esadecimale
#define     RED     0xff0000        //rosso in esadecimale
#define     TRASP   -1
#define     DIM_RAC 70              //dimensioni racchetta
//------------------------------------------------------------------------------
// PUNTI TAVOLO IN PROSPETTIVA
//------------------------------------------------------------------------------
#define     P1_X    100 
#define     P1_Z    420
#define     P2_X    180
#define     P2_Z    100
#define     P3_X    460
#define     P3_Z    100
#define     P4_X    540 
#define     P4_Z    420 
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
//------------------------------------------------------------------------------
// PARAMETRI PALLINA
//------------------------------------------------------------------------------
#define     BALL_RADIUS  5
#define     BALL_COLOR   0xfa6717      //arancione 
//------------------------------------------------------------------------------
// VISTA PROSPETTICA: theta = 45gradi
//------------------------------------------------------------------------------
#define POV_DIST  300
#define COS_THETA 0.707 
#define SIN_THETA 0.707

struct gpos{

    int x;
    int z;

}; 

struct gpos gcord;

int     pview_flag;     //indicatore per rappresentazione prospettica

void init_screen(void);  /* Inizializza lo schermo e crea il task grafico */

void testo(BITMAP* buf);       /* Stampa il testo sullo schermo */

void draw_screen(BITMAP* buf);          /* Disegna l'interfaccia su una bitmap */

void white2pink(BITMAP* b);      /* Converte lo sfondo della sprite in rosa */

void racchetta_avversario(BITMAP* bmp, int w, int h);       /* Disegna una racchetta sullo schermo */

void racchetta_robot(BITMAP* bmp, int w, int h);

void draw_ball(void);           /* Disegna la pallina */

void *display(void* arg);       /* Task grafico */

void display_camera_view(BITMAP* buf);      /* Disegna la vista della camera del robot */

void prospective_view(int x, int y, int z);      /* Genera cordinate per vista prospettica */

void* command(void* arg);          // aggiorna i flag in base ai tasti premuti


