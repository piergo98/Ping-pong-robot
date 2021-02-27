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

            task_create(balltask, 0, 20, 20, 80);
            task_create(camera, 1, 60, 60, 70);
            task_create(motortask_x, 2, 50, 50, 60);
            task_create(motortask_z, 3, 50, 50, 60);
            task_create(adversarytask_x, 4, 50, 50, 60);
            task_create(adversarytask_z, 5, 50, 50, 60);
            //task_create(command, 7, 5, 5, 40);
            task_create(display, 6, 20, 20, 50);

            while(!end){

                /*sem_getvalue(&s3, &sem_3);
                printf("s3 = %d\n", sem_3);
                sem_getvalue(&s4, &sem_4);
                printf("s4 = %d\n", sem_4);
                sem_getvalue(&s5, &sem_5);
                printf("s5 = %d\n", sem_5);
                sem_getvalue(&s6, &sem_6);
                printf("s6 = %d\n", sem_6);
                sem_getvalue(&s7, &sem_7);
                printf("s7 = %d\n", sem_7);
                sem_getvalue(&s8, &sem_8);
                printf("s8 = %d\n", sem_8);
                sem_getvalue(&s9, &sem_9);
                printf("s9 = %d\n", sem_9);
                sem_getvalue(&s10, &sem_10);
                printf("s10 = %d\n", sem_10);
                sem_getvalue(&s11, &sem_11);
                printf("s11 = %d\n", sem_11);
                sem_getvalue(&s11, &sem_12);
                printf("s12 = %d\n", sem_12);
                sem_getvalue(&s13, &sem_13);
                printf("s13 = %d\n", sem_13);
                sem_getvalue(&s14, &sem_14);
                printf("s14 = %d\n", sem_14);
                sem_getvalue(&s15, &sem_15);
                printf("s15 = %d\n", sem_15);
                sem_getvalue(&s16, &sem_16);
                printf("s16 = %d\n", sem_16);
                sem_getvalue(&s17, &sem_17);
                printf("s17 = %d\n", sem_17);
                sem_getvalue(&s18, &sem_18);
                printf("s18 = %d\n", sem_18);
                sem_getvalue(&s19, &sem_19);
                printf("s19 = %d\n", sem_19);
                sem_getvalue(&s20, &sem_20);
                printf("s20 = %d\n", sem_20);
                sem_getvalue(&s21, &sem_21);
                printf("s21 = %d\n", sem_21);
                printf("-------------------------------------------------------\n"); */
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