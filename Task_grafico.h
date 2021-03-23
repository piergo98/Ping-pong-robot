#include    <allegro.h>
#include    <math.h>


#define     COLOR_DEPTH   32              //profondita di colori: 16 bit
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
#define     DIM_RAC 70              //dimensioni racchetta
//------------------------------------------------------------------------------
// PUNTI TAVOLO VISTO DALL'ALTO
//------------------------------------------------------------------------------
#define     P1_X   160  //(X2,Z2)  |-----------------|  (X3,Z3)
#define     P1_Z   420  //         |                 |
#define     P2_X   160  //         |                 |
#define     P2_Z   60   //         |                 |
#define     P3_X   480  //         |                 |
#define     P3_Z   60   //         |                 |
#define     P4_X   480  //         |                 |
#define     P4_Z   420  //(X1,Z1)  |-----------------| (X4,Z4)

#define     HALF_Z  240
#define     HALF_X  320 
//------------------------------------------------------------------------------
// PARAMETRI PALLINA
//------------------------------------------------------------------------------
#define     BALL_RADIUS  5
#define     BALL_COLOR   0xfa6717      //arancione 
//------------------------------------------------------------------------------
// VISTA PROSPETTICA: theta = 30 gradi
//------------------------------------------------------------------------------
#define POV_DIST  600
#define COS_THETA 0.866 //0.866  //0.866
#define SIN_THETA  0.5 //0.5  //-0.5
#define COS_PHI  0.866
#define SIN_PHI  0.5

struct gpos{

    int x;
    int z;

}; 

struct gpos gcord;

int     pview_flag;     //indicatore per rappresentazione prospettica
int pov;
float angle;
int tempo, pos_old, vel_old;

void init_screen(void);  /* Inizializza lo schermo e crea il task grafico */

void testo(BITMAP* buf);       /* Stampa il testo sullo schermo */

void draw_screen(BITMAP* buf);          /* Disegna l'interfaccia su una bitmap */

void white2pink(BITMAP* b);      /* Converte lo sfondo della sprite in rosa */

void racchetta_avversario(BITMAP* bmp, BITMAP* finestra, int w, int h);       /* Disegna una racchetta sullo schermo */

void racchetta_robot(BITMAP* bmp, BITMAP* finestra, int w, int h);

void draw_ball(BITMAP* finestra);           /* Disegna la pallina */

void *display(void* arg);       /* Task grafico */

void display_camera_view(BITMAP* buf);      /* Disegna la vista della camera del robot */

void prospective_view(int x, int y, int z);      /* Genera cordinate per vista prospettica */

void* command(void* arg);          // aggiorna i flag in base ai tasti premuti

void draw_graph(BITMAP* buf);
