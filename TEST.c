#include    "Task_grafico.h"
#include    "Pallina.h"

void init();


int main(void){    

    int     i;
    char    scan;

            init();

            do {
                scan = 0;
                if (keypressed()) scan = readkey() >> 8;
                switch(scan){
                    case KEY_V:
                        pview_flag = 0;
                        break;
                    case KEY_P:
                        pview_flag = 1;
                        break;
                    case KEY_U:
                        player = 1;
                        break;
                    case KEY_R:
                        player = 0;
                        break;
                    default: break; //da aggiungere altre opzioni
                }
            } while(scan != KEY_ESC);
            end = 1;
    
            for (i = 0; i<7; i++) wait_for_end(i);
            allegro_exit();
            return 0;
}

void init(void){

    init_ball();
    init_camera();
    init_motor();
    init_screen();

    task_create(balltask, 0, 40, 40, 80);
    task_create(camera, 1, 40, 40, 70);
    task_create(motortask_x, 2, 50, 50, 60);
    task_create(motortask_z, 3, 50, 50, 60);
    task_create(adversarytask_x, 4, 50, 50, 60);
    task_create(adversarytask_z, 5, 50, 50, 60);
    task_create(display, 6, 100, 100, 81);
    //task_create(command, 7, 30, 30, 40);
}