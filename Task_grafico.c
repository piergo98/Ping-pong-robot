#include    <stdlib.h>
#include    <stdio.h>

#include    "Task_grafico.h"

char    punti_rob[DIM_S];
char    punti_avv[DIM_S];
int     p_rob;          //punteggio robot
int     p_avv;          //punteggio avversario

void init_screen(void){

    allegro_init();
    set_color_depth(COLOR);
    set_gfx_mode(GFX_AUTODETECT_WINDOWED, WIDTH, HEIGTH, 0, 0);
    clear_to_color(screen, BLACK);  //schermo nero
    install_keyboard();
    install_mouse();
}

void draw_screen(void){

    int i;
    int vertici[VERTEX] = {P1_X, P1_Y, P2_X, P2_Y, P3_X, P3_Y, P4_X, P4_Y};
    p_avv = 0;
    p_rob = 0;

    //nasconde il cursore del mouse
    scare_mouse();
    // Disegna il tavolo
    polygon(screen, 4, vertici, FIELD);
    line(screen, 320, 380, 320, 60, WHITE);

    // Disegna la rete
    for (i = 0;i < 381;i += 5){
        line(screen, 130 + i, 200, 130 + i, 170, WHITE);
    }
    for (i = 0;i < 31;i += 5){
        line(screen, 130, 170 + i, 510, 170 + i, WHITE);
    }
    
    unscare_mouse();
    textout_ex(screen, font, "PUNTEGGIO", P_X, P_Y, WHITE, TRASP);
    sprintf(punti_rob, "ROB = %d", p_rob);
    textout_ex(screen, font, punti_rob, P_X, P_Y + 20, WHITE, TRASP);  
    sprintf(punti_avv, "AVV = %d", p_avv);
    textout_ex(screen, font, punti_avv, P_X, P_Y + 40, WHITE, TRASP);
}

void new_bitmap(BITMAP* b, int w, int h){

    b = create_bitmap(w, h);
    /* inizializza la bitmap */
    clear_bitmap(b);
    clear_to_color(b, BLACK);
}

void white2pink(BITMAP* b){

    BITMAP* racp;
    PALETTE pal;
    int     i, j, c;
    int     pink;
    float   hue, sat, val;
    
    pink = makecol(255, 0, 255);
    b = load_bitmap("racchetta.bmp", NULL);
    racp = create_bitmap(b->w, b->h);
    for (i = 0; i < b->w; i++){
        for (j = 0; j < b->h; j++){
            c = getpixel(b, i, j);
            rgb_to_hsv(getr(c), getg(c), getb(c), &hue, &sat, &val);
            val *= 255;
            if (val >= 240) c = pink;
            putpixel(racp, i, j, c);
        }
    }
    get_palette(pal);
    save_bitmap("racp.bmp", racp, pal);
}