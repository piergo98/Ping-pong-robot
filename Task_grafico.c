#include    <stdlib.h>
#include    <stdio.h>

#include    "Task_grafico.h"
#include    "Pallina.h"

char    punti_rob[DIM_S];
char    punti_avv[DIM_S];
int     p_rob;          //punteggio robot
int     p_avv;          //punteggio avversario
BITMAP* rac;            //buffer per copiare lo schermo

void init_screen(void){

    allegro_init();
    set_color_depth(COLOR);
    set_gfx_mode(GFX_AUTODETECT_WINDOWED, WIDTH, HEIGTH, 0, 0);
    clear_to_color(screen, BLACK);  //schermo nero
    install_keyboard();
    install_mouse();
    rac = load_bitmap("racchetta_nosfondo32.bmp", NULL);
    white2pink(rac);
    rac = load_bitmap("racp.bmp", NULL);

    //task_create(display, indice, periodo, drel, priorita);

}

void draw_screen(void){

    int i;
    int vertici[VERTEX] = {P1_X, P1_Y, P2_X, P2_Y, P3_X, P3_Y, P4_X, P4_Y};
    p_avv = 0;
    p_rob = 0;
    BITMAP* buf;

    //crea un buffer per realizzare lo sfondo
    buf = create_bitmap(WIDTH, HEIGTH);
    clear_bitmap(buf);

    //nasconde il cursore del mouse
    scare_mouse();
    // Disegna il tavolo
    polygon(buf, 4, vertici, FIELD);
    line(buf, 320, 420, 320, 100, WHITE);

    // Disegna la rete
    for (i = 0;i < 381;i += 5){
        line(buf, 130 + i, 240, 130 + i, 210, WHITE);
    }
    for (i = 0;i < 31;i += 5){
        line(buf, 130, 210 + i, 510, 210 + i, WHITE);
    }
    
    unscare_mouse();
    textout_ex(buf, font, "PUNTEGGIO", P_X, P_Y, WHITE, TRASP);
    sprintf(punti_rob, "ROB = %d", p_rob);
    textout_ex(buf, font, punti_rob, P_X, P_Y + 20, WHITE, TRASP);  
    sprintf(punti_avv, "AVV = %d", p_avv);
    textout_ex(buf, font, punti_avv, P_X, P_Y + 40, WHITE, TRASP);
    textout_ex(buf, font, "LEGENDA:", X_LEG, P_Y, WHITE, TRASP);
    textout_ex(buf, font, "R -> Gioca robot", X_LEG, P_Y + 20, WHITE, TRASP);
    textout_ex(buf, font, "U -> Gioca utente", X_LEG, P_Y + 40, WHITE, TRASP);
    textout_ex(buf, font, "SPACE -> Battuta", X_LEG, P_Y + 60, WHITE, TRASP);
    textout_ex(buf, font, "utente", X_LEG + 80, P_Y + 70, WHITE, TRASP);
    //trasferisco la bitmap sullo schermo
    blit(buf, screen, 0, 0, 0, 0, WIDTH, HEIGTH);
}

void white2pink(BITMAP* b){

    BITMAP* racp;
    PALETTE pal;
    int     i, j, c;
    int     pink;
    
    pink = makecol(255, 0, 255);
    b = load_bitmap("racchetta_nosfondo32.bmp", NULL);
    racp = create_bitmap(b->w, b->h);
    for (i = 0; i < b->w; i++){
        for (j = 0; j < b->h; j++){
            c = getpixel(b, i, j);
            if (c == WHITE) c = pink;
            putpixel(racp, i, j, c);
        }
    }
    get_palette(pal);
    save_bitmap("racp.bmp", racp, pal);
}

void racchetta(BITMAP* bmp, int w, int h){

    int x, y, x_old, y_old;
    BITMAP* aux, *buf;

    aux = create_bitmap(WIDTH, HEIGTH);
    clear_bitmap(aux);

    x = mouse_x;
    y = mouse_y;
    x_old = x;
    y_old = y;

    while(!keypressed()){

        blit(screen, aux, 0, 0, 0, 0, WIDTH, HEIGTH);
        blit(screen, aux, x, y, x, y, w, h);
        stretch_sprite(screen, bmp, x, y, w, h);
    
        x_old = x;
        y_old = y;
        x = mouse_x;
        y = mouse_y;

        blit(aux, screen, x_old, y_old, x_old, y_old, w, h);
    }    
}

void draw_ball(void)
{
    int x, y;
    
    x = P2_X + ball.x;
    y = P2_Y + ball.y;
    
    circlefill(screen, x, y, BALL_RADIUS, BALL_COLOR);
}

void* display(void* arg){

    int i;      //task index
    int we = 70;
    int he = 70;

    i = get_task_index(arg);
    set_activation(i);

    draw_screen();
    while(1)
        racchetta(rac, we, he);

    if (deadline_miss(i))
        //show_dmiss
    wait_for_activation(i);
}