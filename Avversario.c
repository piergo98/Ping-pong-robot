#include "Avversario.h"

int col = 14;       // yellow color

void* adversarytask_x(void* arg)
{
    int   i, T;                     // task index
    int   x_min, x_max;             // limiti di movimento      
    int   xd, vd;                   // desired position and speed
    int   x,  v;                    // actual  position and speed
    float   u, z, y;                // temporary variables
    struct  state   temp;           //temporary structure
        
        i = get_task_index(arg);
        set_activation(i);
        //sem_wait(&s1);
        T = tp[i].deadline;         //la utilizzo per il rapp. inc.
        //sem_post(&s1);

        x_min = C_X1 - OFFSET_X;
        x_max = C_X4 + OFFSET_X;

        pthread_mutex_lock(&s8);
        temp.position = adversary_x.position;
        temp.speed = adversary_x.speed;
        pthread_mutex_unlock(&s8);

        //sem_wait(&s2);
        while(!end) {

            vd = 0;

            pthread_mutex_lock(&s3);
            xd = 480 - buffer[NEXT].x;
            pthread_mutex_unlock(&s3);

            get_state(&x, &v, &temp);
            u = KP*(xd - x) + KD*(vd - v);
            y = motor(u, &adv_x_angle);
            
            pthread_mutex_lock(&s10);
            update_adversary_state_x(y, T, x_min, x_max, &temp);
            pthread_mutex_unlock(&s10);

            pthread_mutex_lock(&s8);
            adversary_x.position = temp.position;
            adversary_x.speed = temp.speed;
            pthread_mutex_unlock(&s8);
            

            if (deadline_miss(i))
                show_dmiss(i);

            wait_for_activation(i);
        }
        //sem_post(&s2);
}

void* adversarytask_z(void* arg)
{
    int   i, T;                 // task index
    int   z_min, z_max;         // limiti di movimento
    int   zd, vd;               // desired position and speed
    int   x,  v;                // actual  position and speed
    float   u, z, y;            // temporary variables
    struct  state   temp;       //temporary structure
        
        i = get_task_index(arg);
        set_activation(i);
        //sem_wait(&s1);
        T = tp[i].period / 10;       //la utilizzo per il rapp. inc.
        //sem_post(&s1);

        z_min = C_Z1 - OFFSET_Z;
        z_max = C_Z1 + OFFSET_Z / 3;

        pthread_mutex_lock(&s9);
        temp.position = adversary_z.position;
        temp.speed = adversary_z.speed;
        pthread_mutex_unlock(&s9);

        while(!end) {

            vd = 0;

            pthread_mutex_lock(&s3);
            zd = buffer[NEXT].z;
            pthread_mutex_unlock(&s3);

            //pthread_mutex_lock(&s13);
            //zd =ball.z;
            //pthread_mutex_unlock(&s13);

            get_state(&x, &v, &temp);
            u = KP*(zd - x) + KD*(vd - v);
            y = motor(u, &adv_z_angle);

            
            pthread_mutex_lock(&s10);
            update_adversary_state_z(y, T, z_min, z_max, &temp);
            pthread_mutex_unlock(&s10);

            pthread_mutex_lock(&s9);
            adversary_z.position = temp.position;
            adversary_z.speed = temp.speed;
            pthread_mutex_unlock(&s9);

            if (deadline_miss(i))
                show_dmiss(i);
                
            wait_for_activation(i);
        }
        
}

void update_adversary_state_x(float y, int T, int p_min, int p_max, struct state *robot_tmp)
{
  
    int pippo;

    pippo = robot_tmp->position + y * R;                              //converte rotazione del motore in movimento cinghia
    
        if (pippo > p_max )
            robot_tmp->position = p_max;

        else if (pippo < p_min)
            robot_tmp->position = p_min;

        else if (player)
            robot_tmp->position = mouse_x;

        else{
            robot_tmp->speed = ((int)pippo - robot_tmp->position)/ T;         //rapp. incrementale
            robot_tmp->position = (int)pippo;
        } 
}

void update_adversary_state_z(float y, int T, int p_min, int p_max, struct state *robot_tmp)
{
  
    int pippo;

    pippo = robot_tmp->position + y * R;                              //converte rotazione del motore in movimento cinghia
    
        if (pippo > p_max )
            robot_tmp->position = p_max;

        else if (pippo < p_min)
            robot_tmp->position = p_min;

        else if (player)
            robot_tmp->position = mouse_y;

        else{
            robot_tmp->speed = ((int)pippo - robot_tmp->position)/ T;         //rapp. incrementale
            robot_tmp->position = (int)pippo;
        } 
}



