#include    "Task_grafico.h"
#include    "Pallina.h"

void init();


int main(void){    

    int     i,      j;
    char    scan;

    sem_init(&s1, 0, 1);    //semaforo tp
    sem_init(&s2, 0, 1);    //semaforo end
    sem_init(&s3, 0, 1);    //semaforo buffer
    sem_init(&s4, 0, 1);    //semaforo window
    sem_init(&s5, 0, 1);    //semaforo prevtheta
    sem_init(&s6, 0, 1);    //semaforo robot_x
    sem_init(&s7, 0, 1);    //semaforo robot_z
    sem_init(&s8, 0, 1);    //semaforo adversary_x
    sem_init(&s9, 0, 1);    //semaforo adversary_z
    sem_init(&s10, 0, 1);   //semaforo player
    sem_init(&s11, 0, 1);   //semaforo mouse_x_flag
    sem_init(&s12, 0, 1);   //semaforo mouse_z_flag
    sem_init(&s13, 0, 1);   //semaforo ball
    sem_init(&s14, 0, 1);   //semaforo ball_miss
    sem_init(&s15, 0, 1);   //semaforo camera_miss
    sem_init(&s16, 0, 1);   //semaforo motor_x_miss
    sem_init(&s17, 0, 1);   //semaforo motor_z_miss
    sem_init(&s18, 0, 1);   //semaforo adv_x_miss
    sem_init(&s19, 0, 1);   //semaforo adv_z_miss
    sem_init(&s20, 0, 1);   //semaforo display_miss
    sem_init(&s21, 0, 1);   //semaforo tastiera_miss
    

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
    task_create(camera, 1, 20, 20, 70);                //crea problemi il semaforo su prediction()
    task_create(motortask_x, 2, 50, 50, 60);
    task_create(motortask_z, 3, 50, 50, 60);
    task_create(adversarytask_x, 4, 50, 50, 60);
    task_create(adversarytask_z, 5, 50, 50, 60);
    task_create(display, 6, 20, 20, 60);
    //task_create(command, 7, 5, 5, 50);
}