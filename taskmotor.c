#include "taskmotor.h" 

void init_motor(){

    /* Inizializza le posizioni e le velocità dei robot */
    pthread_mutex_lock(&s6);
    robot_x.position = C_X3;
    robot_x.speed = 0;
    pthread_mutex_unlock(&s6);
    
    pthread_mutex_lock(&s7);
    robot_z.position = C_Z3;
    robot_z.speed = 0;
    pthread_mutex_unlock(&s7);
   
    pthread_mutex_lock(&s8);
    adversary_x.position = C_X4;
    adversary_x.speed = 0;
    pthread_mutex_unlock(&s8);
    
    pthread_mutex_lock(&s9);
    adversary_z.position = C_Z4;
    adversary_z.speed = 0;
    pthread_mutex_unlock(&s9);

    pthread_mutex_lock(&s10);
    player = 0;
    pthread_mutex_unlock(&s10);

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

void* motortask_x(void* arg)
{
    int     i,      T;                    // task index
    int     x_min,  x_max;                // limiti di movimento      
    int     xd,     vd;                   // desired position and speed
    int     x,      v;                    // actual  position and speed
    float   u1[2],      y,   err[2],    u;            // temporary variables
    struct  state   temp;                 // temporary structure
        
            i = get_task_index(arg);
            set_activation(i);

            x_min = C_X3 - OFFSET_X;    //70
            x_max = C_X2 + OFFSET_X;    //570

            err[NOW] = err[BEFORE] = 0;
            u1[NOW] = u1[BEFORE]  = 0;

            via =0;

            while(!end) {

                if (!via) {

                    vd = 0;

                    pthread_mutex_lock(&s6);
                    temp.position = robot_x.position;
                    temp.speed = robot_x.speed;
                    pthread_mutex_unlock(&s6);

                    pthread_mutex_lock(&s3);
                    xd = buffer[NEXT].x;
                    pthread_mutex_unlock(&s3);
                
                    get_state(&x, &v, &temp);
                    //errore di posizione
                    err[NOW] = xd-x;
                    //controllo di posizione
                    u1[NOW] = (KP + Ts * KI) * err[NOW] - KP * err[BEFORE] + u1[BEFORE];
                    //controllo di velocita' 
                    u = u1[NOW] + KD * (vd - v);

                    printf("x = %d\t", x);
                    printf("v_x = %d\n", v);

                    y = motor(u, &rob_x_angle);

                    //pthread_mutex_lock(&s11);
                    update_state(y, err[NOW], x_min, x_max, &temp);
                    //pthread_mutex_unlock(&s11);
                    
                    u1[BEFORE] = u1[NOW];
                    err[BEFORE] = err[NOW];
                    
                    pthread_mutex_lock(&s6);
                    robot_x.position = temp.position;
                    robot_x.speed = temp.speed;
                    pthread_mutex_unlock(&s6);

                    if (err[NOW]<=4 && err[NOW]>= -4) via = 1;
                }

                if (deadline_miss(i))
                    show_dmiss(i);

                wait_for_activation(i);
            }
}

void* motortask_z(void* arg)
{
    int     i,      T;                      // task index
    int     z_min,  z_max;                  // limiti di movimento      
    int     xd,     vd;                     // desired position and speed
    int     x,      v;                      // actual  position and speed
    float   u1[2],      y,   err[2],    u;  // temporary variables
    struct  state   temp;                   // temporary structure
        
            i = get_task_index(arg);
            set_activation(i);
            
            T = tp[i].deadline / 10;           //la utilizzo per il rapp. inc.

            z_min = C_Z3 - OFFSET_Z / 3;    //0
            z_max = C_Z3 + OFFSET_Z;        //240

            err[NOW] = err[BEFORE] = 0;
            u1[NOW] = u1[BEFORE]  = 0;

            while(!end) {

                if (via) {

                    vd = 0;

                    pthread_mutex_lock(&s6);
                    temp.position = robot_z.position;
                    temp.speed = robot_z.speed;
                    pthread_mutex_unlock(&s6);

                    pthread_mutex_lock(&s3);
                    xd = buffer[NEXT].z;
                    pthread_mutex_unlock(&s3);
                
                    get_state(&x, &v, &temp);
                    //errore di posizione
                    err[NOW] = xd-x;
                    //controllo di posizione
                    u1[NOW] = (KP + Ts * KI) * err[NOW] - KP * err[BEFORE] + u1[BEFORE];
                    //controllo di velocita' 
                    u = u1[NOW] + KD * (vd - v);

                    printf("z = %d\t", x);
                    printf("v_z = %d\n", v);

                    y = motor(u, &rob_x_angle);

                    //pthread_mutex_lock(&s11);
                    update_state(y, err[NOW], z_min, z_max, &temp);
                    //pthread_mutex_unlock(&s11);

                    u1[BEFORE] = u1[NOW];
                    err[BEFORE] = err[NOW];
                    
                    pthread_mutex_lock(&s7);
                    robot_z.position = temp.position;
                    robot_z.speed = temp.speed;
                    pthread_mutex_unlock(&s7);

                    if (err[NOW]<=4 && err[NOW]>= -4) via =0;
                }

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
    
        theta = A * prevtheta->in[NOW] + B * prevtheta->in[BEFORE] + (1 + P) * prevtheta->out[NOW] - P * prevtheta->out[BEFORE];       //transfer function
    
        prevtheta->in[BEFORE] = prevtheta->in[NOW];
        prevtheta->out[BEFORE] = prevtheta->out[NOW];
        prevtheta->out[NOW] = theta;
        pthread_mutex_unlock(&s5);
    
    return theta;
}

void update_state(float y, float e, int p_min, int p_max, struct state *robot_tmp)
{
    int delta;

    delta = (int)y * R;                              //converte rotazione del motore in movimento cinghia
    
        if (delta > p_max)
            robot_tmp->position = p_max;

        else if (delta < p_min)
            robot_tmp->position = p_min;

        else{
            robot_tmp->speed = ((delta - robot_tmp->position)/ Ts);         //rapp. incrementale
            robot_tmp->position = delta;
        } 
}

void get_state(int *xi, int *vi, struct state *robot_tmp)
{
    *xi = robot_tmp->position;
    *vi = robot_tmp->speed;
}

