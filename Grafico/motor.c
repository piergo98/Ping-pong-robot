#include    "motor.h"

void init_motor(){

    /* Inizializza le posizioni e le velocità dei robot */
    pthread_mutex_lock(&s6);
    robot_x.position = 160;
    robot_x.speed = 0;
    pthread_mutex_unlock(&s6);
    
    pthread_mutex_lock(&s7);
    robot_z.position = 0;
    robot_z.speed = 0;
    pthread_mutex_unlock(&s7);
   
    pthread_mutex_lock(&s8);
    adversary_x.position = 0;
    adversary_x.speed = 0;
    pthread_mutex_unlock(&s8);
    
    pthread_mutex_lock(&s9);
    adversary_z.position = 0;
    adversary_z.speed = 0;
    pthread_mutex_unlock(&s9);

    pthread_mutex_lock(&s10);
    player = 0;
    pthread_mutex_unlock(&s10);
   
    pthread_mutex_lock(&s11);
    target = TARGET;
    pthread_mutex_unlock(&s11);

    rob_x_angle.in[NOW] = 0;      
    rob_x_angle.in[BEFORE] = 0;
    rob_x_angle.out[NOW] = 0;
    rob_x_angle.out[BEFORE] = 0;

    rob_z_angle.in[NOW] = 0;        
    rob_z_angle.in[BEFORE] = 0;
    rob_z_angle.out[NOW] = 0;
    rob_z_angle.out[BEFORE] = 0;

    adv_x_angle.in[NOW] = 0;        
    adv_x_angle.in[BEFORE] = 0;
    adv_x_angle.out[NOW] = 0;
    adv_x_angle.out[BEFORE] = 0;

    adv_z_angle.in[NOW] = 0;        
    adv_z_angle.in[BEFORE] = 0;
    adv_z_angle.out[NOW] = 0;
    adv_z_angle.out[BEFORE] = 0;
}

void* motortask(void* arg)
{
    int     i,      T;                          // task index
    int     x_min,  x_max;                      // limiti di movimento      
    int     xd,     vd;                         // desired position and speed
    int     x,      v;                          // actual  position and speed
    float   u1[2],  y,  err[2], u;              // temporary variables
    struct  state   temp;                       // temporary structure
        
            i = get_task_index(arg);
            set_activation(i);

            x_min = C_X3 - OFFSET_X;    //70
            x_max = C_X2 + OFFSET_X;    //570

            err[NOW] = err[BEFORE] = 0;
            u1[NOW] = u1[BEFORE]  = 0;

            while(!end) {

                vd = 0;

                pthread_mutex_lock(&s6);
                temp.position = robot_x.position;
                temp.speed = robot_x.speed;
                pthread_mutex_unlock(&s6);

                pthread_mutex_lock(&s3);
                //riferimento di posizione
                xd = target;
                pthread_mutex_unlock(&s3);
                
                get_state(&x, &v, &temp);
                // errore di posizione
                err[NOW] = xd-x;
                //controllo di posizione
                u1[NOW] = (KP + Ts * KI) * err[NOW] - KP * err[BEFORE] + u1[BEFORE];
                //controllo di velocita
                u = u1[NOW] + KD * (vd - v);  

                printf("u = %f\t", u);
                printf("e = %f\t", err[NOW]);
                printf("x = %d\t", x);
                printf("v = %d\n", v);
                
                y = motor(u, &rob_x_angle);
                update_state(y, err[NOW], x_min, x_max, &temp);

                u1[BEFORE] = u1[NOW];
                err[BEFORE] = err[NOW];
                
                pthread_mutex_lock(&s6);
                robot_x.position = temp.position;
                robot_x.speed = temp.speed;
                pthread_mutex_unlock(&s6);

                if (deadline_miss(i))
                    show_dmiss(i);

                wait_for_activation(i);
            }
}

float motor(float k, struct m_tfunc *prevtheta)    //angolo di cui deve ruotare il motore per arrivare alla posizione desiderata
{
    float theta;
        
        pthread_mutex_lock(&s5);
        prevtheta->in[NOW] = k;
    
        theta = (A * prevtheta->in[NOW]) + (B * prevtheta->in[BEFORE]) + ((1 + P) * prevtheta->out[NOW]) - (P * prevtheta->out[BEFORE]);       //transfer function
    
        prevtheta->in[BEFORE] = prevtheta->in[NOW];
        prevtheta->out[BEFORE] = prevtheta->out[NOW];
        prevtheta->out[NOW] = theta;
        pthread_mutex_unlock(&s5);
    
    return theta;
}

void update_state(float y, float e, int p_min, int p_max, struct state *robot_tmp)
{
    int delta;

    delta = (int)y * R;           //converte rotazione del motore in movimento cinghia
    
    if (e <= 1 && e >= -1) robot_tmp->speed = 0;
    else robot_tmp->speed = (delta - robot_tmp->position)/ Ts;         //rapp. incrementale
    robot_tmp->position = delta; 
}

void get_state(int *xi, int *vi, struct state *robot_tmp)
{
    *xi = robot_tmp->position;
    *vi = robot_tmp->speed;
}