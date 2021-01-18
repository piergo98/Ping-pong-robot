#include <stdlib.h>
#include <stdio.h>
#include <sched.h>
#include <pthread.h>
#include <time.h>

#define     NT  7            //numero massimo di task
#define     DIM_S   30       //dimensioni stringa

int     ball_miss, camera_miss, motor_x_miss, motor_z_miss, adv_x_miss, adv_z_miss, display_miss, tastiera_miss;



struct task_par{	    //struttura che memorizza tutti i parametri del thread

    int     arg;            //argomento del task
    int     wcet;           //wcet in microsecondi (ms)
    int     period;         //periodo del task (ms)
    int     deadline;       //deadline relativa (ms)
    int     priority;       //priorità del task [0,99]
    int     dmiss;          //numero di deadline misses
    struct  timespec at;    //attivazione successiva
    struct  timespec dl;    //deadline assoluta

};

struct	    task_par	tp[NT];
pthread_t               tid[NT];  
int                     end;                    //end flag

/* Dichiarazione prototipi delle funzioni che effettuano operazioni sulle variabili temporali */

void    time_copy (struct timespec *td, struct timespec ts);

void    time_add_ms (struct timespec *t, int ms);

int     time_cmp (struct timespec t1, struct timespec t2);

/* Dichiarazione prototipi delle funzioni di PTask */

int     task_create (void* (*task) (void *), int i, int period, int drel, int prio);

int     get_task_index (void* arg);

void    set_activation (int i);

int     deadline_miss(int i);

void wait_for_activation(int i);

int wait_for_end(int i);

void show_dmiss(int i);          //mostra il numero di deadline miss