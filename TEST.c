#include    "Task_grafico.h"
#include    "Pallina.h"

void init();


int main(void){    

    int a, i;
    char scan;

    a = 0;

    init();
    do {
        scan = 0;
        if (keypressed()) scan = readkey() >> 8;
        /*if (scan == KEY_SPACE && a == 0){
            task_create(balltask, a, 50, 60, 50);
            a++;
        }*/
    } while(scan != KEY_ESC);
    end = 1;
    for (i = 0; i<7; i++) wait_for_end(i);
    allegro_exit();
    return 0;
}

void init(){

    init_ball();
    init_camera();
    init_motor();
    init_screen();

    task_create(balltask, 0, 50, 60, 50);
    task_create(camera, 1, 40, 40, 40);
    task_create(motortask_x, 2, 45, 45, 30);
    task_create(motortask_z, 3, 45, 45, 30);
    task_create(adversarytask_x, 4, 45, 45, 30);
    task_create(adversarytask_z, 5, 45, 45, 30);
    task_create(display, 6, 25, 25, 20);
}