#include    "Task_grafico.h"


int main(void){

    BITMAP* rac;
    

    init_screen();
    //draw_screen();
    
    display_camera(); 
    
    readkey();
    allegro_exit();
    return 0;
}