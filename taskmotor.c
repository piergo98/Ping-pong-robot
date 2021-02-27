#include "taskmotor.h" 

void* motortask_x(void* arg)
{
    int     i,      T;                    // task index
    int     x_min,  x_max;                // limiti di movimento      
    int     xd,     vd;                   // desired position and speed
    int     x,      v;                    // actual  position and speed
    float   u,      z,      y;            // temporary variables
    struct  state   temp;                 // temporary structure
        
            i = get_task_index(arg);
            set_activation(i);
            //sem_wait(&s1);
            T = tp[i].deadline;           //la utilizzo per il rapp. inc.
            //sem_post(&s1);

            x_min = C_X3 - OFFSET_X;    //70
            x_max = C_X2 + OFFSET_X;    //570

            pthread_mutex_lock(&s5);
            prevtheta.in[NOW] = 0;        //sarebbe stato piu' bello un for?
            prevtheta.in[BEFORE] = 0;
            prevtheta.out[NOW] = 0;
            prevtheta.out[BEFORE] = 0;
            pthread_mutex_unlock(&s5);

            pthread_mutex_lock(&s6);
            temp.position = robot_x.position;
            temp.speed = robot_x.speed;
            pthread_mutex_unlock(&s6);

            //sem_wait(&s2);
            while(!end) {

                vd = 0;

                pthread_mutex_lock(&s3);
                xd = buffer[NEXT].x;
                pthread_mutex_unlock(&s3);
                /*pthread_mutex_lock(&s13);
                xd = ball.x;
                pthread_mutex_unlock(&s13);*/

                get_state(&x, &v, &temp);
                u = KP*(xd - x) + KD*(vd - v);
                y = motor(u);
                update_state(y, T, x_min, x_max, &temp);
                
                pthread_mutex_lock(&s6);
                robot_x.position = temp.position;
                robot_x.speed = temp.speed;
                pthread_mutex_unlock(&s6);

                if (deadline_miss(i))
                    show_dmiss(i);

                wait_for_activation(i);
            }
            //sem_post(&s2);
}

void* motortask_z(void* arg)
{
    int     i,      T;              // task index
    int     z_min,  z_max;          // limiti di movimento
    int     xd,     vd;             // desired position and speed
    int     x,      v;              // actual  position and speed
    float   u,      z,      y;      // temporary variables
    struct  state   temp;           //temporary structure

        
            i = get_task_index(arg);
            set_activation(i);
            //sem_wait(&s1);
            T = tp[i].period / 10;       //la utilizzo per il rapp. inc.
            //sem_post(&s1);

            z_min = C_Z3 - OFFSET_Z / 3;
            z_max = C_Z3 + OFFSET_Z;

            pthread_mutex_lock(&s5);
            prevtheta.in[NOW] = 0;
            prevtheta.in[BEFORE] = 0;
            prevtheta.out[NOW] = 0;
            prevtheta.out[BEFORE] = 0;
            pthread_mutex_unlock(&s5);

            pthread_mutex_lock(&s7);
            temp.position = robot_z.position;
            temp.speed = robot_z.speed;
            pthread_mutex_unlock(&s7);

            //sem_wait(&s2);
            while(!end) {

                vd = 0;

                pthread_mutex_lock(&s3);
                xd = buffer[NEXT].z;
                pthread_mutex_unlock(&s3);
                /*pthread_mutex_lock(&s13);
                xd = ball.z;
                pthread_mutex_unlock(&s13);*/

                get_state(&x, &v, &temp);
                u = KP*(xd - x) + KD*(vd - v);
                y = motor(u);
                update_state(y, T, z_min, z_max, &temp);
                
                pthread_mutex_lock(&s7);
                robot_z.position = temp.position;
                robot_z.speed = temp.speed;
                pthread_mutex_unlock(&s7);

                if (deadline_miss(i))
                    show_dmiss(i);
                    
                wait_for_activation(i);
            }
            //sem_post(&s2);
}

float motor(float k)    //angolo di cui deve ruotare il motore per arrivare alla posizione desiderata
{
    float theta;
        
        pthread_mutex_lock(&s5);
        prevtheta.in[NOW] = k;
    
        theta = A * prevtheta.in[NOW] + B * prevtheta.in[BEFORE] + (1 + P) * prevtheta.out[NOW] - P * prevtheta.out[BEFORE];       //transfer function
    
        prevtheta.in[BEFORE] = prevtheta.in[NOW];
        prevtheta.out[BEFORE] = prevtheta.out[NOW];
        prevtheta.out[NOW] = theta;
        pthread_mutex_unlock(&s5);
    
    return theta;
}

void update_state(float y, int T, int p_min, int p_max, struct state *robot_tmp)
{
    y = robot_tmp->position + y * R;                              //converte rotazione del motore in movimento cinghia
    
    //pthread_mutex_lock(&s11);
    //pthread_mutex_lock(&s12);
        if (y > p_max)
            robot_tmp->position = p_max;

        else if (y < p_min)
            robot_tmp->position = p_min;

        else if (mouse_x_flag == 1)
            robot_tmp->position = mouse_x;

        else if (mouse_z_flag == 1)
            robot_tmp->position = mouse_y;

        else{
            robot_tmp->speed = ((int)y - robot_tmp->position)/ T;         //rapp. incrementale
            robot_tmp->position = (int)y;
        }
    //pthread_mutex_unlock(&s12);
    //pthread_mutex_unlock(&s11);  
}

void get_state(int *xi, int *vi, struct state *robot_tmp)
{
    *xi = robot_tmp->position;
    *vi = robot_tmp->speed;
}

void init_motor(){

    /* Inizializza le posizioni e le velocità dei robot */
    pthread_mutex_lock(&s6);
    robot_x.position = C_X1;
    robot_x.speed = 0;
    pthread_mutex_unlock(&s6);
    
    pthread_mutex_lock(&s7);
    robot_z.position = C_Z2;
    robot_z.speed = 0;
    pthread_mutex_unlock(&s7);
   
    pthread_mutex_lock(&s8);
    adversary_x.position = C_X2;
    adversary_x.speed = 0;
    pthread_mutex_unlock(&s8);
    
    pthread_mutex_lock(&s9);
    adversary_z.position = C_Z1;
    adversary_z.speed = 0;
    pthread_mutex_unlock(&s9);

    pthread_mutex_lock(&s10);
    player = 0;
    pthread_mutex_unlock(&s10);
   
    pthread_mutex_lock(&s11);
    mouse_x_flag = 0;
    pthread_mutex_unlock(&s11);
    
    pthread_mutex_lock(&s12);
    mouse_z_flag = 0;
    pthread_mutex_unlock(&s12);
}