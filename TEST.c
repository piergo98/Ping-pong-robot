#include    "Task_grafico.h"
#include    "Pallina.h"

void init();


int main(void){    

    int a, i;

    i = a = 0;
    init();
    
    //readkey();
    allegro_exit();
    return 0;
}

void init(){

    init_ball();
    init_camera();
    init_motor();
    init_screen();
    
    task_create(balltask, 1, 50, 60, 50);
    task_create(camera, 2, 40, 40, 40);
    task_create(motortask_x, 3, 45, 45, 30);
    task_create(motortask_z, 4, 45, 45, 30);
    task_create(adversarytask_x, 5, 45, 45, 30);
    task_create(adversarytask_z, 6, 45, 45, 30);
    task_create(display, 7, 25, 25, 20);
}