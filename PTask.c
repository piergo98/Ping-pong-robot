#include "PTask.h"

/*Definizione delle funzioni che operano sulle variabili temporali*/

void time_copy (struct timespec *td, struct timespec ts){     //copia ts nella variabile temporale puntata da td

    td->tv_sec  =   ts.tv_sec;
    td->tv_nsec =   ts.tv_nsec;

}

void time_add_ms (struct timespec *t, int ms){         //aggiunge ms (in millisecondi) alla variabile puntata da t

    t->tv_sec   +=  ms/1000;
    t->tv_nsec  +=  (ms%1000)*1000000;

    if (t->tv_nsec  >   1000000000){
        t->tv_sec   +=  1;
        t->tv_nsec  -=  1000000000;  
    }
}

int  time_cmp (struct timespec t1, struct timespec t2){       //compara due variabili e ritorna 0 se t1 = t2, 1 se t1 > t2, -1 se t1 < t2

    if (t1.tv_sec   >   t2.tv_sec) return 1;
    if (t1.tv_sec   <   t2.tv_sec) return -1;
    if (t1.tv_nsec  >   t2.tv_nsec) return 1;
    if (t1.tv_nsec  <   t2.tv_nsec) return -1;
    return 0;
}

/*Definizione delle funzioni di PTASK*/

int task_create (void* (*task) (void *), int i, int period, int drel, int prio){    //crea un task

    pthread_attr_t myatt;	    //definisco un puntatore alla struttura degli attributi
    struct sched_param mypar;	//definisco la struttura mypar, del tipo sched_param
    int tret;	

    if (i > NT) return -1;

    tp[i].arg = i;
    tp[i].period = period;
    tp[i].deadline = drel;
    tp[i].priority = prio;
    tp[i].dmiss = 0;
    pthread_attr_init (&myatt);
    pthread_attr_setinheritsched (&myatt, PTHREAD_EXPLICIT_SCHED);
    pthread_attr_setschedpolicy (&myatt, SCHED_RR);
    mypar.sched_priority = tp[i].priority;
    pthread_attr_setschedparam (&myatt, &mypar);	//applica le modifiche agli attributi del thread

    tret = pthread_create (&tid[i], &myatt, task, (void*)(&tp[i]));

    return tret;
}

int get_task_index (void* arg){     //restituisce l'indice del task

    struct task_par *tpar;

    tpar = (struct task_par *) arg;
    return tpar->arg;
}

void set_activation (int i){        //legge il tempo corrente e calcola l'attivazione successiva e la deadline assoluta

    struct timespec t;

    clock_gettime(CLOCK_MONOTONIC, &t);
    time_copy(&(tp[i].at), t);
    time_copy(&(tp[i].dl), t);
    time_add_ms(&(tp[i].at), tp[i].period);
    time_add_ms(&(tp[i].dl), tp[i].deadline);
}

int deadline_miss (int i){          //conta il numero di deadline misses

    struct timespec now;

    clock_gettime(CLOCK_MONOTONIC, &now);
    if (time_cmp(now, tp[i].dl) > 0){
        tp[i].dmiss++;
        return 1;
    }
    return 0;
}

void wait_for_activation (int i){   //sospende l'esecuzione del thread fino alla prossima attivazione

    clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &(tp[i].at), NULL);
    time_add_ms(&(tp[i].at), tp[i].period);
    time_add_ms(&(tp[i].dl), tp[i].deadline);
}

int wait_for_end(int i){    //attende la terminazione del task

    return pthread_join(tid[i], NULL);
}

void show_dmiss(int i){

    char    string[DIM_S];

            switch(i){

                case 0:
                    ball_miss += 1;
                    break;
                case 1:
                    camera_miss += 1;
                    break;
                case 2:
                    motor_x_miss += 1;
                    break;
                case 3:
                    motor_z_miss += 1;
                    break;
                case 4:
                    adv_x_miss += 1;
                    break;
                case 5:
                    adv_z_miss += 1;
                    break;
                case 6:
                    display_miss += 1;
                    break;
                case 7:
                    tastiera_miss += 1;
                    break;
                default: break;
            }
}