#include    <allegro.h>
#include    <math.h>

//------------------------------------------------------------------------------
// COSTANTI INIZIALIZZAZIONE SCHERMO
//------------------------------------------------------------------------------
#define     WIDTH           1024
#define     HEIGHT          768
#define     COLOR_DEPTH     32              //profondita di colori: 32 bit
#define     WIDTH_GRAPH     384
#define     HEIGHT_GRAPH    768
//------------------------------------------------------------------------------
// PARAMETRI LEGENDA
//------------------------------------------------------------------------------
#define     P_X             20
#define     P_Z             20
#define     X_LEG           200
#define     Z_DIST          20
//------------------------------------------------------------------------------
// COLORI A 32 BIT
//------------------------------------------------------------------------------
#define     FIELD           0x9acd32        //colore del campo in esadecimale
#define     WHITE           0xffffff        //bianco in esadecimale
#define     BLACK           0x000000        //nero in esadecimale
#define     RED             0xff0000        //rosso in esadecimale
#define     BLUE            0x0000ff        //blu in esadecimale
#define     GREEN           0x00ff00        //verde in esadecimale
#define     FUCSIA          0xff00ff        //fucsia in esadecimale
#define     TRASP           -1              //trasparente
#define     BALL_COLOR      0xfa6717        //arancione
//------------------------------------------------------------------------------
// PARAMETRI PER LA RAPPRESENTAZIONE DEL CAMPO
//------------------------------------------------------------------------------
#define     VERTEX          8               //dimensione vettore dei vertici del campo
#define     Z1_V            548             //z1 linea verticale del campo
#define     Z2_V            188             //z2 linea verticale del campo
#define     X1_O            140             //x1 linea di metà campo
#define     X2_O            500             //x2 linea di metà campo
//------------------------------------------------------------------------------
// PARAMETRI PROSPETTIVA: theta = 30 gradi
//------------------------------------------------------------------------------
#define     DIM_RAC         70              //dimensioni racchetta
#define     POV_DIST        1000
#define     COS_THETA       0.866 //0.866  //0.866
#define     SIN_THETA       0.5 //0.5  //-0.5
//------------------------------------------------------------------------------
// PARAMETRI GRAFICI
//------------------------------------------------------------------------------
#define     TEMPO_MAX       384
#define     TEMPO_INIT      0
#define     H_GRAPH_1       192
#define     H_GRAPH_2       384
#define     H_GRAPH_3       576
#define     H_GRAPH_4       768
#define     SCALE           5               // fattore di scala
#define     X_GRAPH         280             // X legenda dei grafici
#define     Z_GRAPH_1       20              // Z legenda dei grafici
#define     Z_GRAPH_2       212             // Z legenda dei grafici
#define     Z_GRAPH_3       404             // Z legenda dei grafici
#define     Z_GRAPH_4       596             // Z legenda dei grafici
//------------------------------------------------------------------------------

struct gpos{

    int x;
    int z;
}; 

struct gpos gcord;

int     pview_flag;     //indicatore per rappresentazione prospettica
int     pov;
float   angle;
int     tempo, pos_old_1, pos_old_2, pos_old_3, pos_old_4;

void init_screen(void);  /* Inizializza lo schermo e crea il task grafico */

void legenda(BITMAP* buf);       /* Stampa il testo sullo schermo */

void draw_screen(BITMAP* buf);          /* Disegna l'interfaccia su una bitmap */

void white2pink(BITMAP* b);      /* Converte lo sfondo della sprite in rosa */

void racchetta_avversario(BITMAP* bmp, BITMAP* finestra, int w, int h);       /* Disegna una racchetta sullo schermo */

void racchetta_robot(BITMAP* bmp, BITMAP* finestra, int w, int h);

void draw_ball(BITMAP* finestra);           /* Disegna la pallina */

void *display(void* arg);       /* Task grafico */

void display_camera_view(BITMAP* buf);      /* Disegna la vista della camera del robot */

void prospective_view(int x, int y, int z);      /* Genera cordinate per vista prospettica */

void* command(void* arg);          // aggiorna i flag in base ai tasti premuti

void draw_graph(BITMAP* buf);       //disegna i grafici dei motori
