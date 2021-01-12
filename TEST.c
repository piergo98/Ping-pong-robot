#include    "Task_grafico.h"


int main(void){

    BITMAP* rac;
    int x, y;
    int we = 50;
    int he = 50;

    init_screen();
    draw_screen();
    //--------------------------------------------                                        
    //carica l'icona della racchetta
    //--------------------------------------------
    rac = load_bitmap("racchetta.bmp", NULL);
    white2pink(rac);
    rac = load_bitmap("racp.bmp", NULL);
    while(!keypressed()){
        x = mouse_x;
        y = mouse_y;
        stretch_sprite(screen, rac, x, y, we, he);
        draw_sprite(screen, rac, x, y);
        //show_mouse(screen);
    }    
    //readkey();
    allegro_exit();
    return 0;
}