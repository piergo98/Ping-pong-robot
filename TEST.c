#include    "Task_grafico.h"


int main(void){

    BITMAP* rac;
    int we = 50;
    int he = 50;

    init_screen();
    draw_screen();
    //--------------------------------------------                                        
    //carica l'icona della racchetta
    //--------------------------------------------
    rac = load_bitmap("racchetta_nosfondo32.bmp", NULL);
    white2pink(rac);
    rac = load_bitmap("racp.bmp", NULL);
    racchetta(rac, we, he); 

    allegro_exit();
    return 0;
}