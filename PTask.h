#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>

#define NT  10            //numero massimo di thread

/*#ifndef PTask_h
#define PTask_h
#endif*/

struct task_par{	    //struttura che memorizza tutti i parametri del thread

    int     arg;            //argomento del task
    int     wcet;           //wcet in microsecondi
    int     period;         //periodo del task
    int     deadline;       //deadline relativa (ms)
    int     priority;       //priorità del task
    int     dmiss;          //numero di deadline misses
    struct  timespec at;        //attivazione successiva
    struct  timespec dl;        //deadline assoluta

};

extern struct	task_par	tp[NT];

extern          pthread_t   tid[NT];   

/* Dichiarazione prototipi delle funzioni che effettuano operazioni sulle variabili temporali */

void    time_copy (struct timespec *td, struct timespec ts);

void    time_add_ms (struct timespec *t, int ms);

int     time_cmp (struct timespec t1, struct timespec t2);

/* Dichiarazione prototipi delle funzioni di PTask */

void    *task (void *arg);

int     task_create (void* (*task) (void *), int i, int period, int drel, int prio);

int     get_task_index (void* arg);

void    set_activation (int i);

int     deadline_miss(int i);

void wait_for_activation(int i);