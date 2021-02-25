#include "Pallina.h"

int tflag = 0;                  //trail flag
int tl = 10;                    //actual trail lenght
//int end = 0;                    //end flag
float g = G0;                   //acceleration of gravity


void* balltask(void* arg)
{
    int i;                  // task index
    float dt;               // integration interval
        
        i = get_task_index(arg);
        set_activation(i);
        
        //init_ball();
        sem_wait(&s1);
        dt = TSCALE* (float)tp[i].period /1000;
        sem_post(&s1);
        
        while (!end) {
            
            sem_wait(&s13);
            ball.x  += ball.vx * dt - BETA * ball.vx * dt;
            ball.y  += 0.01*(Y_0 + ball.vy * dt - g * dt * dt / 2);
            ball.z  -= ball.vz * dt - BETA * ball.vz * dt;
            sem_post(&s13);

            handle_bounce(i);
            store_trail(i);

            if (deadline_miss(i))
                show_dmiss(i);
                
            wait_for_activation(i);
        }
}

void store_trail( int i)
{ 
    int k;

    k = trail.top;
    k = (k + 1) % TLEN;
    sem_wait(&s13);
    trail.x[k] = ball.x;
    trail.z[k] = ball.z;
    trail.y[k] = ball.y;
    sem_post(&s13);
    trail.top = k;
}

void handle_bounce(int i)           //gestioni dei rimbalzi quando incontra racchetta e il tavolo
{
    /* Gestioni rimbalzo con la racchetta del robot */
    sem_wait(&s6);
    sem_wait(&s7);
    sem_wait(&s13);
    if ((robot_z.position >= ball.z - ball.r) && (ball.x - ball.r >= robot_x.position) && (ball.x + ball.r <= robot_x.position + RACC_MAX))
    {
        ball.vz = - ball.vz + robot_z.speed;
        ball.vx = - ball.vx + robot_x.speed + frand(ERR_MIN, ERR_MAX);              //robusto a rimbalzo dritto continuo
    }
    sem_post(&s13);
    sem_post(&s7);
    sem_post(&s6);
    

    /* Gestioni rimbalzo con la racchetta dell'avversario */
    sem_wait(&s8);
    sem_wait(&s9);
    sem_wait(&s13);
    if ((adversary_z.position >= ball.z + ball.r) && (ball.x - ball.r >= adversary_x.position) && (ball.x + ball.r <= adversary_x.position + RACC_MAX))
    {
        ball.vz = - ball.vz + adversary_z.speed;
        ball.vx = - ball.vx + adversary_x.speed + frand(ERR_MIN, ERR_MAX);          //robusto a rimbalzo dritto continuo
    }
    sem_post(&s9);
    sem_post(&s8);

    if (ball.y == 0)
    {
        ball.vy = -ball.vy;
    }
    sem_post(&s13);
}

float frand(float vxmin, float vxmax)
{
    float r;
    
    r = rand() / (float)RAND_MAX;   // rand in [0,1)
    
    return vxmin + (vxmax - vxmin) * r;
}

void init_ball(void)
{
   sem_wait(&s13); 
   ball.vx = 0;
   ball.vy = 0;
   ball.vz = 5;
   
   ball.x = 320;
   ball.y = Y_0;
   ball.z = 60;
   sem_post(&s13);
}