#include    "Task_grafico.h"
#include    "Pallina.h"


int main(void){    

    int a, i;

    i = a = 0;
    init_screen();      

    while(!key[KEY_ESC]){

        if (a == 0 && key[KEY_V]){
            a = 1;
            i = 0;
        }
        else if (a == 1 && key[KEY_V]){
            a = 0;
            i = 0;
        }
        if (a == 0 && i == 0)
            draw_screen();
        else if(a == 1 && i == 0)
            display_camera();
        i = 1;
    }
    
    //readkey();
    allegro_exit();
    return 0;
}