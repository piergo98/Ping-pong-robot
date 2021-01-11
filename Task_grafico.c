#include    "Task_grafico.h"


void init_screen(){

    BITMAP* rac;
    //int vertici[VERTEX] = {160, 360, 200, 120, 440, 120, 480, 360};
    int vertici[VERTEX] = {120, 420, 200, 60, 440, 60, 520, 420};
    p_avv = 0;
    p_rob = 0;

    allegro_init();
    install_keyboard();
    install_mouse();
    set_color_depth(COLOR);
    set_gfx_mode(GFX_AUTODETECT_WINDOWED, WIDTH, HEIGTH, 0, 0);
    clear_to_color(screen, 0);  //schermo nero
    scare_mouse();              //nasconde il cursore del mouse
    polygon(screen, 4, vertici, FIELD);
    line(screen, 150, 220, 490, 220, WHITE);
    /* disegnare la rete evetualmente */
    unscare_mouse();
    textout_ex(screen, font, "PUNTEGGIO", P_X, P_Y, WHITE, TRASP);
    sprintf(punti_rob, "ROB = %d", p_rob);
    textout_ex(screen, font, punti_rob, P_X, P_Y + 50, WHITE, TRASP);   //da finire
    sprintf(punti_avv, "AVV = %d", p_avv);
    textout_ex(screen, font, punti_avv, P_X, P_Y + 100, WHITE, TRASP);   //da finire
    //carica l'icona della racchetta
    rac = load_bitmap("racchetta.bmp", NULL);
    set_mouse_sprite(rac);
    //set_mouse_sprite_focus()

}