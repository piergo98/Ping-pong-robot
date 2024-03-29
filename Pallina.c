#include "Pallina.h"

float g = G0;                   //acceleration of gravity


void* balltask(void* arg)
{
    int i;                  // task index
    float dt;               // integration interval
        
        i = get_task_index(arg);
        set_activation(i);
        
        dt = (float)tp[i].period /100;
        
        while (!end) {
            
            pthread_mutex_lock(&s13);
            ball.x  += ball.vx * dt - BETA * ball.vx * dt;
            ball.y = Y_0;
            ball.z  += ball.vz * dt - BETA * ball.vz * dt;
            pthread_mutex_unlock(&s13);

            handle_bounce(i);

            if (ball.x > WIDTH_GAME || ball.x < 0 || ball.z > HEIGHT_GAME || ball.z < DELTA){

                if (home) {
                    
                    pthread_mutex_lock(&s2);
                    p_avv++;   
                    pthread_mutex_unlock(&s2);
                }
                else {
                   
                    pthread_mutex_lock(&s1);
                    p_rob++;
                    pthread_mutex_unlock(&s1);
                }             
                
                init_ball();
                init_motor();
                pthread_mutex_lock(&s11);
                start = 0;
                pthread_mutex_unlock(&s11);
            }

            if (deadline_miss(i))
                show_dmiss(i);
                
            wait_for_activation(i);
        }
}

void handle_bounce(int i)           //gestioni dei rimbalzi quando incontra racchetta e il tavolo
{
    /* Gestioni rimbalzo con la racchetta del robot */
    pthread_mutex_lock(&s6);
    pthread_mutex_lock(&s7);
    pthread_mutex_lock(&s13);
    pthread_mutex_lock(&s12);
    if ((robot_z.position - EPS <= ball.z - ball.r) && (robot_z.position + D >= ball.z - ball.r) && (ball.x + ball.r >= robot_x.position - RACC_MIN) && (ball.x - ball.r <= robot_x.position + RACC_MAX))
    {
        ball.z = robot_z.position + 2 * ball.r;
        ball.vz = -ball.vz + DAMP_Z * robot_z.speed;
        ball.vx = - ball.vx + DAMP_X * robot_x.speed + frand(ERR_MIN, ERR_MAX);     //robusto a rimbalzo dritto continuo
        home = 0;
    }
    pthread_mutex_unlock(&s12);
    pthread_mutex_unlock(&s13);
    pthread_mutex_unlock(&s7);
    pthread_mutex_unlock(&s6);
    

    /* Gestioni rimbalzo con la racchetta dell'avversario */
    pthread_mutex_lock(&s8);
    pthread_mutex_lock(&s9);
    pthread_mutex_lock(&s13);
    pthread_mutex_lock(&s12);
    if ((adversary_z.position - D <= ball.z + ball.r) && (adversary_z.position + EPS >= ball.z + ball.r) && (ball.x + ball.r >= adversary_x.position - RACC_MIN) && (ball.x - ball.r <= adversary_x.position + RACC_MAX))
    {
        ball.z = adversary_z.position -  2 * ball.r;
        ball.vz = -ball.vz + DAMP_Z * adversary_z.speed;
        ball.vx = - ball.vx + DAMP_X * adversary_x.speed + frand(ERR_MIN, ERR_MAX);          //robusto a rimbalzo dritto continuo
        home = 1;
    }
    pthread_mutex_unlock(&s12);
    pthread_mutex_unlock(&s13);
    pthread_mutex_unlock(&s9);
    pthread_mutex_unlock(&s8);
    
    pthread_mutex_lock(&s13);
    if (ball.y == 0)
    {
        ball.vy = -ball.vy;
    }
    pthread_mutex_unlock(&s13);
}

float frand(float vxmin, float vxmax)
{
    float r;
    
    r = rand() / (float)RAND_MAX;   // rand in [0,1)
    
    return vxmin + (vxmax - vxmin) * r;
}

void init_ball(void)
{
   pthread_mutex_lock(&s13); 
   ball.vx = 0;
   ball.vy = 0;
   ball.vz = 0;
   
   ball.x = INIT_X;
   ball.y = Y_0;
   ball.z = INIT_Z;

   ball.r = BALL_RADIUS;
   pthread_mutex_unlock(&s13);
}