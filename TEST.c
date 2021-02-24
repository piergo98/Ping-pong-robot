#include    "Task_grafico.h"
#include    "Pallina.h"

void init();


int main(void){    

    int     i;
    char    scan;

            init();
    
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
    task_create(display, 6, 20, 20, 60);
    //task_create(command, 7, 10, 10, 50);
}