#include "taskmotor.h" 

void* motortask_x(void* arg)
{
    int     i,      T;                    // task index
    int     x_min,  x_max;                // limiti di movimento      
    int     xd,     vd;                   // desired position and speed
    int     x,      v;                    // actual  position and speed
    float   u,      z,      y;            // temporary variables
        
            i = get_task_index(arg);
            set_activation(i);
            T = tp[i].deadline;           //la utilizzo per il rapp. inc.

            x_min = C_X3 - OFFSET_X;
            x_max = C_X2 + OFFSET_X;

            sem_wait(&s5);
            prevtheta.in[NOW] = 0;        //sarebbe stato piu' bello un for?
            prevtheta.in[BEFORE] = 0;
            prevtheta.out[NOW] = 0;
            prevtheta.out[BEFORE] = 0;
            sem_post(&s5);

        while(!end) {

            vd = 0;

            sem_wait(&s3);
            xd = buffer[NOW].x;
            sem_post(&s3);

            sem_wait(&s6);
            get_state(&x, &v, &robot_x);
            u = KP*(xd - x) + KD*(vd - v);
            //z = delay(u);
            y = motor(z);
            update_state(y, T, x_min, x_max, &robot_x);
            sem_post(&s6);

            if (deadline_miss(i))
                show_dmiss(i);

            wait_for_activation(i);
        }
}

void* motortask_z(void* arg)
{
    int     i,      T;         // task index
    int     z_min,  z_max;             // limiti di movimento
    int     xd,     vd;       // desired position and speed
    int     x,      v;        // actual  position and speed
    float   u,      z,      y;    // temporary variables
        
            i = get_task_index(arg);
            set_activation(i);
            T = tp[i].period / 10;       //la utilizzo per il rapp. inc.

            z_min = C_Z3 - OFFSET_Z / 3;
            z_max = C_Z3 + OFFSET_Z;

            sem_wait(&s5);
            prevtheta.in[NOW] = 0;
            prevtheta.in[BEFORE] = 0;
            prevtheta.out[NOW] = 0;
            prevtheta.out[BEFORE] = 0;
            sem_post(&s5);

        while(!end) {

            vd = 0;

            sem_wait(&s3);
            xd = buffer[NEXT].z;
            sem_post(&s3);

            sem_wait(&s7);
            get_state(&x, &v, &robot_z);
            u = KP*(xd - x) + KD*(vd - v);
            //z = delay(u);
            y = motor(z);
            update_state(y, T, z_min, z_max, &robot_z);
            sem_post(&s7);

            if (deadline_miss(i))
                show_dmiss(i);
                
            wait_for_activation(i);
        }
}

float motor(float k)
{
    float theta;
        
        sem_wait(&s5);
        prevtheta.in[NOW] = k;
    
        theta = A * prevtheta.in[NOW] + B * prevtheta.in[BEFORE] + (1 + P) * prevtheta.out[NOW] - P * prevtheta.out[BEFORE];       //transfer function
    
        prevtheta.in[BEFORE] = prevtheta.in[NOW];
        prevtheta.out[BEFORE] = prevtheta.out[NOW];
        prevtheta.out[NOW] = theta;
        sem_post(&s5);
    
    return theta;
}

void update_state(float y, int T, int p_min, int p_max, struct state *robot_tmp)
{
    y = robot_tmp->position + y * R;                              //converte rotazione del motore in movimento cinghia
    
    robot_tmp->speed = ((int)y - robot_tmp->position)/ T;         //rapp. incrementale
    
    sem_wait(&s11);
    sem_wait(&s12);
        if (y > p_max)
            robot_tmp->position = p_max;

        else if (y < p_min)
            robot_tmp->position = p_min;

        else if (mouse_x_flag == 1)
            robot_tmp->position = mouse_x;

        else if (mouse_z_flag == 1)
            robot_tmp->position = mouse_y;

        else
            robot_tmp->position = (int)y; 
    sem_post(&s12);
    sem_post(&s11);  
}

void get_state(int *xi, int *vi, struct state *robot_tmp)
{
    *xi = robot_tmp->position;
    *vi = robot_tmp->speed;
}

void init_motor(){

    sem_wait(&s6);
    sem_wait(&s7);
    sem_wait(&s8);
    sem_wait(&s9);
    /* Inizializza le posizioni dei robot */
    robot_x.position = C_X1;
    robot_z.position = C_Z2;
    adversary_x.position = C_X2;
    adversary_z.position = C_Z1;

    /* Inizializza le velocita' dei robot */
    robot_x.speed = 0;
    robot_z.speed = 0;
    adversary_x.speed = 0;
    adversary_z.speed = 0;

    sem_wait(&s10);
    player = 1;
    sem_post(&s10);
    sem_wait(&s11);
    sem_wait(&s12);
    mouse_x_flag = 0;
    mouse_z_flag = 0;
    sem_post(&s12);
    sem_post(&s11);
    sem_post(&s9);
    sem_post(&s8);
    sem_post(&s7);
    sem_post(&s6);
}