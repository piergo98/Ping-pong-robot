#include "Pallina.h"

int tflag = 0;                  //trail flag
int tl = 10;                    //actual trail lenght
int end = 0;                    //end flag
float g = G0;                     //acceleration of gravity


void* balltask(void* arg)
{
    int i;                  // task index
    float dt;               // integration interval
        
        i = get_task_index(arg);
        
        init_ball();
        dt = TSCALE* (float)tp[i].period /1000;
        
        while (!end) {
            
            ball.y;                                         //scrivere legge di y

            ball.x  += ball.vx * dt - BETA * ball.vx * dt;
            ball.z  += ball.vz * dt - BETA * ball.vz * dt;
            
            handle_bounce(i);
            store_trail(i);
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

void handle_bounce(int i)                                                           //rimbalza quando incontra racchetta
{
    if (ball.z <= robot_z.position + ball.r && ball.x <= robot_x.position + (RACC_MAX - RACC_MIN)/2 && ball.x >= robot_x.position - (RACC_MAX - RACC_MIN)/2)
    {
        ball.z = ball.r;
        ball.vz = - ball.vz + robot_z.speed;
        ball.vx = - ball.vx + robot_x.speed + frand(ERR_MIN, ERR_MAX);              //robusto a rimbalzo dritto continuo
    }

    if (ball.z <= adversary_z.position + ball.r && ball.x <= adversary_x.position + (RACC_MAX - RACC_MIN)/2 && ball.x >= adversary_x.position - (RACC_MAX - RACC_MIN)/2)
    {
        ball.z = adversary_z.position - ball.r;                                     //da sistemare le cordinate assegnate!!!
        ball.vz = - ball.vz + adversary_z.speed;
        ball.vx = - ball.vx + adversary_x.speed + frand(ERR_MIN, ERR_MAX);          //robusto a rimbalzo dritto continuo
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
   ball.vx = 0;
   ball.vy = 0;
   ball.vz = 0;
   
   ball.x = 0;
   ball.y = 0;
   ball.z = 0; 

   return;
}