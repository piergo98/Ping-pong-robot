#include "Pallina.h"

int tflag = 0;                  //trail flag
int tl = 10;                    //actual trail lenght
int end = 0;                    //end flag
float g = G0;                   //acceleration of gravity


void* balltask(void* arg)
{
    int i;                  // task index
    float dt;               // integration interval
        
        i = get_task_index(arg);
        set_activation(i);
        
        //init_ball();
        dt = TSCALE* (float)tp[i].period /1000;
        
        while (!end) {

            ball.x  += ball.vx * dt - BETA * ball.vx * dt;
            ball.y  += Y_0 + ball.vy * dt - g * dt * dt / 2;
            ball.z  += ball.vz * dt - BETA * ball.vz * dt;
            
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
    trail.x[k] = ball.x;
    trail.z[k] = ball.z;
    trail.y[k] = ball.y;
    trail.top = k;
}

void handle_bounce(int i)           //gestioni dei rimbalzi quando incontra racchetta e il tavolo
{
    /* Gestioni rimbalzo con la racchetta del robot */
    if ((robot_z.position >= ball.z - ball.r) && (ball.x - ball.r >= robot_x.position) && (ball.x + ball.r <= robot_x.position + RACC_MAX))
    {
        ball.vz = - ball.vz + robot_z.speed;
        ball.vx = - ball.vx + robot_x.speed + frand(ERR_MIN, ERR_MAX);              //robusto a rimbalzo dritto continuo
    }

    /* Gestioni rimbalzo con la racchetta dell'avversario */
    if ((adversary_z.position >= ball.z + ball.r) && (ball.x - ball.r >= adversary_x.position) && (ball.x + ball.r <= adversary_x.position + RACC_MAX))
    {
        ball.vz = - ball.vz + adversary_z.speed;
        ball.vx = - ball.vx + adversary_x.speed + frand(ERR_MIN, ERR_MAX);          //robusto a rimbalzo dritto continuo
    }

    if (ball.y == 0)
    {
        ball.vy = -ball.vy;
    }
}

float frand(float vxmin, float vxmax)
{
    float r;
    
    r = rand() / (float)RAND_MAX;   // rand in [0,1)
    
    return vxmin + (vxmax - vxmin) * r;
}

void init_ball(void)
{
   ball.vx = 10;
   ball.vy = 0;
   ball.vz = -10;
   
   ball.x = 320;
   ball.y = Y_0;
   ball.z = 330;
}