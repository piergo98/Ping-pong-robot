#include "Pallina.h"

int tflag = 0;                  //trail flag
int tl = 10;                    //actual trail lenght
float g = G0;                   //acceleration of gravity


void* balltask(void* arg)
{
    int i;                  // task index
    float dt;               // integration interval
        
        i = get_task_index(arg);
        set_activation(i);
        
        //init_ball();
        //sem_wait(&s1);
        dt = TSCALE* (float)tp[i].period /1000;
        //sem_post(&s1);
        
        //sem_wait(&s2);
        while (!end) {
            
            pthread_mutex_lock(&s13);
            ball.x  += ball.vx * dt - BETA * ball.vx * dt;
            ball.y  += 0.01*(Y_0 + ball.vy * dt - g * dt * dt / 2);
            ball.z  += 1*(ball.vz * dt - BETA * ball.vz * dt);
            pthread_mutex_unlock(&s13);

            handle_bounce(i);
            store_trail(i);

            if (ball.x > 640 || ball.x < 0 || ball.z > 480 || ball.z < 0){
                //new_ball++;
                //autokill(i);
                init_ball();
            }

            if (deadline_miss(i))
                show_dmiss(i);
                
            wait_for_activation(i);
        }
        //sem_post(&s2);
}

void store_trail( int i)
{ 
    int k;

    k = trail.top;
    k = (k + 1) % TLEN;
    pthread_mutex_lock(&s13);
    trail.x[k] = ball.x;
    trail.z[k] = ball.z;
    trail.y[k] = ball.y;
    pthread_mutex_unlock(&s13);
    trail.top = k;
}

void handle_bounce(int i)           //gestioni dei rimbalzi quando incontra racchetta e il tavolo
{
    /* Gestioni rimbalzo con la racchetta del robot */
    pthread_mutex_lock(&s6);
    pthread_mutex_lock(&s7);
    pthread_mutex_lock(&s13);
    if ((robot_z.position - 5 <= ball.z - ball.r) && (robot_z.position + 5 >= ball.z - ball.r) && (ball.x - ball.r >= robot_x.position) && (ball.x + ball.r <= robot_x.position + RACC_MAX))
    {
        ball.vz = - ball.vz + robot_z.speed;
        ball.vx = - ball.vx + robot_x.speed + frand(ERR_MIN, ERR_MAX);              //robusto a rimbalzo dritto continuo
    }
    pthread_mutex_unlock(&s13);
    pthread_mutex_unlock(&s7);
    pthread_mutex_unlock(&s6);
    

    /* Gestioni rimbalzo con la racchetta dell'avversario */
    pthread_mutex_lock(&s8);
    pthread_mutex_lock(&s9);
    pthread_mutex_lock(&s13);
    if ((adversary_z.position - 5 <= ball.z + ball.r) && (adversary_z.position + 5 >= ball.z + ball.r) && (ball.x - ball.r >= adversary_x.position) && (ball.x + ball.r <= adversary_x.position + RACC_MAX))
    {
        ball.vz = - ball.vz + adversary_z.speed;
        ball.vx = - ball.vx + adversary_x.speed + frand(ERR_MIN, ERR_MAX);          //robusto a rimbalzo dritto continuo
    }
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
   
   ball.x = 320;
   ball.y = Y_0;
   ball.z = 120;

   ball.r = 5;          //BALL_RADIUS
   pthread_mutex_unlock(&s13);
}