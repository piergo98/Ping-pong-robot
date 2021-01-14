#include    "Task_grafico.h"


int main(void){

    BITMAP* rac;
    int we = 70;
    int he = 70;

    init_screen();
    draw_screen();
    //--------------------------------------------                                        
    //carica l'icona della racchetta
    //--------------------------------------------
    rac = load_bitmap("racchetta_nosfondo32.bmp", NULL);
    white2pink(rac);
    rac = load_bitmap("racp.bmp", NULL);
    draw_ball();
    racchetta(rac, we, he); 
    
    //readkey();

    allegro_exit();
    return 0;
}