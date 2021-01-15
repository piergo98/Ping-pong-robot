#include    "Task_grafico.h"


int main(void){

    BITMAP* rac;
    

    init_screen();
    draw_screen();
    //--------------------------------------------                                        
    //carica l'icona della racchetta
    //--------------------------------------------
    draw_ball(); 
    
    //readkey();

    allegro_exit();
    return 0;
}