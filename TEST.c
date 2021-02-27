#include    "Task_grafico.h"
#include    "Pallina.h"

#define _GNU_SOURCE

#define     PRIO_BALL 80
#define     PRIO_CAMERA 70
#define     PRIO_MOTOR_X 60
#define     PRIO_MOTOR_Z 60
#define     PRIO_ADV_X 60
#define     PRIO_ADV_Z 60
#define     PRIO_KEY 40
#define     PRIO_DISPLAY 50

void init();
void init_mutex();


int main(void){    

    int     i,      j,  sem_3, sem_4, sem_5, sem_6, sem_7, sem_8, sem_9, sem_10, sem_11, sem_12, sem_13, sem_14, sem_15, sem_16, sem_17, sem_18, sem_19, sem_20, sem_21;
    char    scan;

            
    
            init();

            task_create(balltask, 0, 40, 40, PRIO_BALL);
            task_create(camera, 1, 40, 40, PRIO_CAMERA);
            task_create(motortask_x, 2, 50, 50, PRIO_MOTOR_X);
            task_create(motortask_z, 3, 50, 50, PRIO_MOTOR_Z);
            task_create(adversarytask_x, 4, 50, 50, PRIO_ADV_X);
            task_create(adversarytask_z, 5, 50, 50, PRIO_ADV_Z);
            //task_create(command, 7, 5, 5, PRIO_KEY);
            task_create(display, 6, 20, 20, PRIO_DISPLAY);

            j = 8;
            while(!end){
                if (new_ball) {
                    init_ball();
                    task_create(balltask, j, 40, 40, PRIO_BALL);
                    new_ball = 0;
                    j++;
                }
            }
        
            for (i = 0; i<7; i++) wait_for_end(i);
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

    pthread_mutexattr_destroy(&mux_att);

    sem_init(&s1, 1, 1);    //semaforo tp
    sem_init(&s2, 1, 1);    //semaforo end
    sem_init(&s14, 1, 1);   //semaforo ball_miss
    sem_init(&s15, 1, 1);   //semaforo camera_miss
    sem_init(&s16, 1, 1);   //semaforo motor_x_miss
    sem_init(&s17, 1, 1);   //semaforo motor_z_miss
    sem_init(&s18, 1, 1);   //semaforo adv_x_miss
    sem_init(&s19, 1, 1);   //semaforo adv_z_miss
    sem_init(&s20, 1, 1);   //semaforo display_miss
    sem_init(&s21, 1, 1);   //semaforo tastiera_miss */
}