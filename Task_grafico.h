#include    <allegro.h>

#define     D       2               //Dimesione buffer di BITMAP
#define     WIDTH   640
#define     HEIGTH  480
#define     COLOR   32              //profondita di colori: 16 bit
#define     VERTEX  8
#define     P_X     20
#define     P_Y     20
#define     FIELD   0x9acd32        //colore del campo in esadecimale
#define     WHITE   0xffffff        //bianco in esadecimale
#define     BLACK   0x000000        //nero in esadecimale
#define     TRASP   -1
#define     DIM_S   10              //dimensioni stringa punteggio
//------------------------------------------------------------------------------
// PUNTI TAVOLO
//------------------------------------------------------------------------------
#define     P1_X    100 
#define     P1_Y    380
#define     P2_X    180
#define     P2_Y    60
#define     P3_X    460
#define     P3_Y    60
#define     P4_X    540 
#define     P4_Y    380    
//------------------------------------------------------------------------------
// PARAMETRI PALLINA
//------------------------------------------------------------------------------
#define BALL_RADIUS  10
#define BALL_COLOR   0xFFC060 //arancione 


void init_screen(void); //inizializza lo schermo

void new_bitmap(BITMAP* b, int w, int h);  /* Crea un bitmap di dimensioni (w x h) */

void draw_screen(void);          /* Disegna l'interfaccia */

void white2pink(BITMAP* b);      /* Converte lo sfondo della sprite in rosa */

void racchetta(BITMAP* bmp, int w, int h);       /* Disegna una racchetta sullo schermo */

void draw_ball(int i);           /* Disegna la pallina */

