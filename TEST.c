#include    "Task_grafico.h"
#include    "Pallina.h"

void init();


int main(void){    

    int     i,      j,  sem_3, sem_4, sem_5, sem_6, sem_7, sem_8, sem_9, sem_10, sem_11, sem_12, sem_13, sem_14, sem_15, sem_16, sem_17, sem_18, sem_19, sem_20, sem_21;
    char    scan;

            //sem_init(&s1, 0, 1);    //semaforo tp
            //sem_init(&s2, 0, 1);    //semaforo end
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

            //task_create(balltask, 0, 10, 10, 80);
            task_create(camera, 1, 20, 20, 70);
            //task_create(motortask_x, 2, 20, 20, 60);
            //task_create(motortask_z, 3, 20, 20, 60);
            //task_create(adversarytask_x, 4, 20, 20, 60);
            //task_create(adversarytask_z, 5, 20, 20, 60);
            //task_create(command, 7, 5, 5, 40);
            task_create(display, 6, 25, 25, 50);

            while(!end){

                sem_getvalue(&s3, &sem_3);
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
                printf("-------------------------------------------------------\n");
            }
    
            for (i = 0; i<7; i++) wait_for_end(i);
            allegro_exit();
            return 0;
}

void init(void){

    end = 0;

    init_ball();
    init_camera();
    init_motor();
    init_screen();    
}