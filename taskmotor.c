#include "taskmotor.h" 

void init_motor(){

    /* Inizializza le posizioni e le velocità dei robot */
    pthread_mutex_lock(&s6);
    robot_x.position = P2_X;
    robot_x.speed = 0;
    pthread_mutex_unlock(&s6);
    
    pthread_mutex_lock(&s7);
    robot_z.position = P2_Z;
    robot_z.speed = 0;
    pthread_mutex_unlock(&s7);
   
    pthread_mutex_lock(&s8);
    adversary_x.position = P4_X;
    adversary_x.speed = 0;
    pthread_mutex_unlock(&s8);
    
    pthread_mutex_lock(&s9);
    adversary_z.position = P4_Z;
    adversary_z.speed = 0;
    pthread_mutex_unlock(&s9);

    pthread_mutex_lock(&s11);
    start = 0;
    pthread_mutex_unlock(&s11);

    pthread_mutex_lock(&s12);
    home = 1;
    pthread_mutex_unlock(&s12);

    rob_x_angle.in[NOW] = 0;      
    rob_x_angle.in[BEFORE] = 0;
    rob_x_angle.out[NOW] = P2_X / R;
    rob_x_angle.out[BEFORE] =  P2_X / R;

    rob_z_angle.in[NOW] = 0;        
    rob_z_angle.in[BEFORE] = 0;
    rob_z_angle.out[NOW] = P2_Z / R;
    rob_z_angle.out[BEFORE] = P2_Z / R;

    adv_x_angle.in[NOW] = 0;        
    adv_x_angle.in[BEFORE] = 0;
    adv_x_angle.out[NOW] = P4_X / R;
    adv_x_angle.out[BEFORE] = P4_X / R;

    adv_z_angle.in[NOW] = 0;        
    adv_z_angle.in[BEFORE] = 0;
    adv_z_angle.out[NOW] = P4_Z / R;
    adv_z_angle.out[BEFORE] = P4_Z / R;
}

void* motortask_x(void* arg)
{
    int     i,      T;                                  // task index      
    int     xd,     vd;                                 // desired position and speed
    int     x,      v;                                  // actual  position and speed
    float   u1[DIM_B],      y,   err[DIM_B],    u;      // temporary variables
    struct  state   temp;                               // temporary structure
        
            i = get_task_index(arg);
            set_activation(i);

            err[NOW] = err[BEFORE] = 0;
            u1[NOW] = u1[BEFORE]  = 0;


            while(!end) {

                if (start) {
                    vd = 0;

                    pthread_mutex_lock(&s6);
                    temp.position = robot_x.position;
                    temp.speed = robot_x.speed;
                    pthread_mutex_unlock(&s6);

                    pthread_mutex_lock(&s12);
                    if (!home)
                    {
                        xd = HALF_X;                    
                    
                    }
                    else {
                        pthread_mutex_lock(&s3);
                        xd = buffer[NEXT].x;
                        pthread_mutex_unlock(&s3);
                    }
                    pthread_mutex_unlock(&s12);
                
                    get_state(&x, &v, &temp);
                    //errore di posizione
                    err[NOW] = xd-x;
                    //controllo di posizione
                    u1[NOW] = (KP + Ts * KI) * err[NOW] - KP * err[BEFORE] + u1[BEFORE];
                    //controllo di velocita' 
                    u = u1[NOW] + KD * (vd - v);

                    y = motor(u, &rob_x_angle);

                    update_state_x(y, err[NOW], &temp);
                    
                    u1[BEFORE] = u1[NOW];
                    err[BEFORE] = err[NOW];
                    
                    pthread_mutex_lock(&s6);
                    robot_x.position = temp.position;
                    robot_x.speed = temp.speed;
                    pthread_mutex_unlock(&s6);
                }
                
                if (deadline_miss(i))
                    show_dmiss(i);

                wait_for_activation(i);
            }
}

void* motortask_z(void* arg)
{
    int     i,      T;                                  // task index     
    int     zd,     vd;                                 // desired position and speed
    int     x,      v;                                  // actual  position and speed
    float   u1[DIM_B],      y,   err[DIM_B],    u;      // temporary variables
    struct  state   temp;                               // temporary structure
        
            i = get_task_index(arg);
            set_activation(i);

            err[NOW] = err[BEFORE] = 0;
            u1[NOW] = u1[BEFORE]  = 0;

            while(!end) {

                if (start) {
                    vd = 0;

                    pthread_mutex_lock(&s7);
                    temp.position = robot_z.position;
                    temp.speed = robot_z.speed;
                    pthread_mutex_unlock(&s7);

                    pthread_mutex_lock(&s12);
                    if (!home) {

                        zd = P2_Z;
                    }
                    else {
                        pthread_mutex_lock(&s3);
                        zd = buffer[NEXT].z - D;
                        pthread_mutex_unlock(&s3);
                    }
                    pthread_mutex_unlock(&s12);
                
                    get_state(&x, &v, &temp);
                    //errore di posizione
                    err[NOW] = zd-x;
                    //controllo di posizione
                    u1[NOW] = (KP + Ts * KI) * err[NOW] - KP * err[BEFORE] + u1[BEFORE];
                    //controllo di velocita' 
                    u = u1[NOW] + KD * (vd - v);

                    y = motor(u, &rob_z_angle);

                    update_state_z(y, err[NOW], &temp);

                    u1[BEFORE] = u1[NOW];
                    err[BEFORE] = err[NOW];
                    
                    pthread_mutex_lock(&s7);
                    robot_z.position = temp.position;
                    robot_z.speed = temp.speed;
                    pthread_mutex_unlock(&s7);
                }

                if (deadline_miss(i))
                    show_dmiss(i);

                wait_for_activation(i);
            }
}

float motor(float k, struct m_tfunc *prevtheta)    //angolo di cui deve ruotare il motore per arrivare alla posizione desiderata
{
    float theta;
        
        prevtheta->in[NOW] = k;
    
        theta = A * prevtheta->in[NOW] + B * prevtheta->in[BEFORE] + (1 + P) * prevtheta->out[NOW] - P * prevtheta->out[BEFORE];       //transfer function
    
        prevtheta->in[BEFORE] = prevtheta->in[NOW];
        prevtheta->out[BEFORE] = prevtheta->out[NOW];
        prevtheta->out[NOW] = theta;
    
    return theta;
}

void update_state_x(float y, float e, struct state *robot_tmp)
{
    int delta;

    delta = (int)y * R;                              //converte rotazione del motore in movimento cinghia
    
        if (delta > X_MAX)
            robot_tmp->position = X_MAX;

        else if (delta < X_MIN)
            robot_tmp->position = X_MIN;

        else{
            robot_tmp->speed = ((delta - robot_tmp->position)/ Ts);         //rapp. incrementale
            robot_tmp->position = delta;
        } 
}

void update_state_z(float y, float e, struct state *robot_tmp)
{
    int delta;

    delta = (int)y * R;                              //converte rotazione del motore in movimento cinghia
    
        if (delta > Z_MAX)
            robot_tmp->position = Z_MAX;

        else if (delta < Z_MIN)
            robot_tmp->position = Z_MIN;

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

