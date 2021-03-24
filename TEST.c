#include    "Task_grafico.h"
#include    "Pallina.h"

#define _GNU_SOURCE
#define NUM_TASK    9
//--------------------------------------------------------------------------------
// PRIORITA' DEI TASK
//--------------------------------------------------------------------------------
#define     PRIO_BALL 80
#define     PRIO_CAMERA 50
#define     PRIO_MOTOR_X 62
#define     PRIO_MOTOR_Z 61
#define     PRIO_ADV_X 60
#define     PRIO_ADV_Z 59
#define     PRIO_KEY 40
#define     PRIO_DISPLAY 70
//--------------------------------------------------------------------------------
// PERIODI DEI TASK
//--------------------------------------------------------------------------------
#define     PER_BALL 20
#define     PER_CAMERA 20
#define     PER_MOTOR_X 20
#define     PER_MOTOR_Z 20
#define     PER_ADV_X 20
#define     PER_ADV_Z 20
#define     PER_KEY 20
#define     PER_DISPLAY 20
//--------------------------------------------------------------------------------
// DEADLINE DEI TASK
//--------------------------------------------------------------------------------
#define     D_BALL 20
#define     D_CAMERA 20
#define     D_MOTOR_X 20
#define     D_MOTOR_Z 20
#define     D_ADV_X 20
#define     D_ADV_Z 20
#define     D_KEY 20
#define     D_DISPLAY 20


void init();
void init_mutex();


int main(void){    

    int     i, j;
    char    scan;

            
    
            init();

            task_create(balltask, 0, PER_BALL, D_BALL, PRIO_BALL);
            task_create(camera, 1, PER_CAMERA, D_CAMERA, PRIO_CAMERA);
            task_create(motortask_x, 2, PER_MOTOR_X, D_MOTOR_X, PRIO_MOTOR_X);
            task_create(motortask_z, 3, PER_MOTOR_Z, D_MOTOR_Z, PRIO_MOTOR_Z);
            task_create(adversarytask_x, 4, PER_ADV_X, D_ADV_X, PRIO_ADV_X);
            task_create(adversarytask_z, 5, PER_ADV_Z, D_ADV_Z, PRIO_ADV_Z);
            task_create(command, 7, PER_KEY, D_KEY, PRIO_KEY);
            task_create(display, 6, PER_DISPLAY, D_DISPLAY, PRIO_DISPLAY);
            //task_create(miss_stamp, 8, 20, 20, 81);
        
            for (i = 0; i < NUM_TASK; i++) 
                wait_for_end(i);
            
            allegro_exit();
            return 0;
}

void init(void){

    end = 0;
    
    pthread_mutex_lock(&s10);
    player = 0;
    pthread_mutex_unlock(&s10);

    init_mutex();
    init_ball();
    init_camera();
    init_motor();
    init_screen();    
}

void init_mutex(void) {

    pthread_mutexattr_init(&mux_att);
    pthread_mutexattr_setprotocol(&mux_att, PTHREAD_PRIO_PROTECT);

    pthread_mutexattr_setprioceiling(&mux_att, PRIO_BALL);      //P_ROB
    pthread_mutex_init(&s1, &mux_att);

    pthread_mutexattr_setprioceiling(&mux_att, PRIO_BALL);      //P_AVV
    pthread_mutex_init(&s2, &mux_att);
    
    pthread_mutexattr_setprioceiling(&mux_att, PRIO_CAMERA);    //BUFFER
    pthread_mutex_init(&s3, &mux_att);

    pthread_mutexattr_setprioceiling(&mux_att, PRIO_CAMERA);    //WINDOW
    pthread_mutex_init(&s4, &mux_att);

    pthread_mutexattr_setprioceiling(&mux_att, PRIO_MOTOR_X);   //(NON USATO)
    pthread_mutex_init(&s5, &mux_att);
    
    pthread_mutexattr_setprioceiling(&mux_att, PRIO_BALL);      //ROBOT_X
    pthread_mutex_init(&s6, &mux_att);

    pthread_mutexattr_setprioceiling(&mux_att, PRIO_BALL);      //ROBOT_Z
    pthread_mutex_init(&s7, &mux_att);

    pthread_mutexattr_setprioceiling(&mux_att, PRIO_BALL);      //ADVERSARY_X
    pthread_mutex_init(&s8, &mux_att);

    pthread_mutexattr_setprioceiling(&mux_att, PRIO_BALL);      //ADVERSARY_Z
    pthread_mutex_init(&s9, &mux_att);

    pthread_mutexattr_setprioceiling(&mux_att, PRIO_MOTOR_X);   //PLAYER
    pthread_mutex_init(&s10, &mux_att);

    pthread_mutexattr_setprioceiling(&mux_att, PRIO_MOTOR_X);   //START
    pthread_mutex_init(&s11, &mux_att);

    pthread_mutexattr_setprioceiling(&mux_att, PRIO_MOTOR_X);   //HOME
    pthread_mutex_init(&s12, &mux_att);

    pthread_mutexattr_setprioceiling(&mux_att, PRIO_BALL);      //BALL
    pthread_mutex_init(&s13, &mux_att);
    
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