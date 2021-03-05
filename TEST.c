#include    "Task_grafico.h"
#include    "Pallina.h"

#define _GNU_SOURCE

#define     PRIO_BALL 80
#define     PRIO_CAMERA 50
#define     PRIO_MOTOR_X 60
#define     PRIO_MOTOR_Z 60
#define     PRIO_ADV_X 60
#define     PRIO_ADV_Z 60
#define     PRIO_KEY 40
#define     PRIO_DISPLAY 70

void init();
void init_mutex();


int main(void){    

    int     i, j;
    char    scan;

            
    
            init();

            task_create(balltask, 0, 40, 40, PRIO_BALL);
            task_create(camera, 1, 50, 50, PRIO_CAMERA);
            task_create(motortask_x, 2, 50, 50, PRIO_MOTOR_X);
            task_create(motortask_z, 3, 50, 50, PRIO_MOTOR_Z);
            //task_create(adversarytask_x, 4, 50, 50, PRIO_ADV_X);
            //task_create(adversarytask_z, 5, 50, 50, PRIO_ADV_Z);
            task_create(command, 7, 30, 30, PRIO_KEY);
            task_create(display, 6, 35, 35, PRIO_DISPLAY);
            task_create(miss_stamp, 8, 25, 25, 81);
        
            for (i = 0; i<9; i++) 
                wait_for_end(i);
            
            allegro_exit();
            return 0;
}

void init(void){

    end = 0;

    init_mutex();
    init_ball();
    init_camera();
    init_motor();
    init_screen();    
}

void init_mutex(void) {

    pthread_mutexattr_init(&mux_att);
    pthread_mutexattr_setprotocol(&mux_att, PTHREAD_PRIO_PROTECT);

    
    pthread_mutexattr_setprioceiling(&mux_att, PRIO_CAMERA);
    pthread_mutex_init(&s3, &mux_att);

    pthread_mutexattr_setprioceiling(&mux_att, PRIO_CAMERA);
    pthread_mutex_init(&s4, &mux_att);

    pthread_mutexattr_setprioceiling(&mux_att, PRIO_MOTOR_X);
    pthread_mutex_init(&s5, &mux_att);
    
    pthread_mutexattr_setprioceiling(&mux_att, PRIO_BALL);
    pthread_mutex_init(&s6, &mux_att);

    pthread_mutexattr_setprioceiling(&mux_att, PRIO_BALL);
    pthread_mutex_init(&s7, &mux_att);

    pthread_mutexattr_setprioceiling(&mux_att, PRIO_BALL);
    pthread_mutex_init(&s8, &mux_att);

    pthread_mutexattr_setprioceiling(&mux_att, PRIO_BALL);
    pthread_mutex_init(&s9, &mux_att);

    pthread_mutexattr_setprioceiling(&mux_att, PRIO_MOTOR_X);
    pthread_mutex_init(&s10, &mux_att);

    pthread_mutexattr_setprioceiling(&mux_att, PRIO_MOTOR_X);
    pthread_mutex_init(&s11, &mux_att);

    pthread_mutexattr_setprioceiling(&mux_att, PRIO_MOTOR_X);
    pthread_mutex_init(&s12, &mux_att);

    pthread_mutexattr_setprioceiling(&mux_att, PRIO_BALL);
    pthread_mutex_init(&s13, &mux_att);

    //sem_init(&s1, 1, 1);    //semaforo tp
    //sem_init(&s2, 1, 1);    //semaforo end
    
    pthread_mutexattr_setprioceiling(&mux_att, PRIO_BALL);
    pthread_mutex_init(&s14, &mux_att);   //semaforo ball_miss

    pthread_mutexattr_setprioceiling(&mux_att, PRIO_CAMERA);
    pthread_mutex_init(&s15, &mux_att);   //semaforo camera_miss

    pthread_mutexattr_setprioceiling(&mux_att, PRIO_MOTOR_X);
    pthread_mutex_init(&s16, &mux_att);   //semaforo motor_x_miss

    pthread_mutexattr_setprioceiling(&mux_att, PRIO_MOTOR_Z);
    pthread_mutex_init(&s17, &mux_att);   //semaforo motor_z_miss

    pthread_mutexattr_setprioceiling(&mux_att, PRIO_ADV_X);
    pthread_mutex_init(&s18, &mux_att);   //semaforo adv_x_miss

    pthread_mutexattr_setprioceiling(&mux_att, PRIO_ADV_Z);
    pthread_mutex_init(&s19, &mux_att);   //semaforo adv_z_miss

    pthread_mutexattr_setprioceiling(&mux_att, PRIO_DISPLAY);
    pthread_mutex_init(&s20, &mux_att);   //semaforo display_miss

    pthread_mutexattr_setprioceiling(&mux_att, PRIO_KEY);
    pthread_mutex_init(&s21, &mux_att);   //semaforo tastiera_miss

    pthread_mutexattr_destroy(&mux_att);
}