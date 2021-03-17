#include    "Robot_camera.h"

void init_camera(){

    /* Inizializzo la struttura puntata da window */
    pthread_mutex_lock(&s4);
    window.x0 = 160;                // punto in alto a sinistra della finestra
    window.z0 = 60;
    window.xsize = SIZE_X;          // dimensioni della finestra
    window.zsize = SIZE_Z;
    pthread_mutex_unlock(&s4);

    pthread_mutex_lock(&s3);
    buffer[BEFORE].x = 0;
    buffer[BEFORE].z = 0;
    buffer[NOW].x = 0;
    buffer[NOW].z = 0;
    buffer[NEXT].x = 0;
    buffer[NEXT].z = 0;
    pthread_mutex_unlock(&s3);
}

int centroid(struct win w, struct coord *target){

    int c, i, j;
    int sample, xc, zc;

    sample = xc = zc = 0;

    for (i = 0; i < w.xsize; i++){
        for (j = 0; j < w.zsize; j++){
            c = getpixel(screen, i + w.x0, j + w.z0);
            if (c == TARGET_COLOR){
                sample++;
                xc += (i + w.x0 - xc) / sample;    /* calcolo la media incrementale sia per x che per y */
                zc += (j + w.z0 - zc) / sample;
            }
        }
    }
    if (sample > THRESHOLD){            //target trovato
            target->x = xc;
            target->z = zc;
            return 1;
        }
        return 0;                       //target non trovato    
}

void prediction(struct win* w, struct coord memory[DIM]){

    int i, r, trovato;

    memory[BEFORE].x = memory[NOW].x;
    memory[BEFORE].z = memory[NOW].z;    

    trovato = centroid(*w, &memory[NOW]);
   
    /* predizione lungo x */
    memory[NEXT].x = (2 * memory[NOW].x) - memory[BEFORE].x;   //0.7
    /* predizione lungo z */
    memory[NEXT].z = (2 * memory[NOW].z) - memory[BEFORE].z;   //0.9
    
    

    if (trovato){
        
        w->xsize = 50;
        w->zsize = 50;
        /* sposto la finestra di ricerca */
        w->x0 = memory[NEXT].x - (w->xsize / 2);
        //w->x0 = memory[NOW].x;
        w->z0 = memory[NEXT].z - (w->zsize / 2);
        //w->z0 = memory[NEXT].z;
    }
    else
    {
        w->xsize = SIZE_X;
        w->zsize = SIZE_Z;
        w->x0 = 160;
        w->z0 = 60;
    }
}

void* camera(void* arg){

    int             i;          //task index
    int             xd;         //posizione desiderata
    struct coord    temp[DIM];  //struttura di appoggio
    struct win      finestra;   //struttura di appoggio

        i = get_task_index(arg);
        set_activation(i);

        //centroid(finestra, &temp[NOW]);
        //centroid(finestra, &temp[NEXT]);

        while(!end){

            pthread_mutex_lock(&s3);
            temp[BEFORE].x = buffer[BEFORE].x;
            temp[BEFORE].z = buffer[BEFORE].z;
            temp[NOW].x = buffer[NOW].x;
            temp[NOW].z = buffer[NOW].z;
            temp[NEXT].x = buffer[NEXT].x;
            temp[NEXT].z = buffer[NEXT].z;
            pthread_mutex_unlock(&s3);

            pthread_mutex_lock(&s4);
            finestra.x0 = window.x0;
            finestra.z0 = window.z0;
            finestra.xsize = window.xsize;
            finestra.zsize = window.zsize;
            pthread_mutex_unlock(&s4);
            
            prediction(&finestra, temp);

            pthread_mutex_lock(&s3);
            buffer[BEFORE].x = temp[BEFORE].x;
            buffer[BEFORE].z = temp[BEFORE].z;
            buffer[NOW].x = temp[NOW].x;
            buffer[NOW].z = temp[NOW].z;
            buffer[NEXT].x = temp[NEXT].x;
            buffer[NEXT].z = temp[NEXT].z;
            pthread_mutex_unlock(&s3);

            pthread_mutex_lock(&s4);
            window.x0 = finestra.x0;
            window.z0 = finestra.z0;
            window.xsize = finestra.xsize;
            window.zsize = finestra.zsize;
            pthread_mutex_unlock(&s4);

            if (deadline_miss(i))
                show_dmiss(i);
                
            wait_for_activation(i);
        }
}

void *miss_stamp(void *arg) {

    int i;

    i = get_task_index(arg);
    set_activation(i);

    while (!end)
    {   

        pthread_mutex_lock(&s14);
        printf("ball = %d \n", ball_miss);
        pthread_mutex_unlock(&s14);
        
        pthread_mutex_lock(&s15);
        printf("camera = %d \n", camera_miss);
        pthread_mutex_unlock(&s15);
        
        pthread_mutex_lock(&s16);
        printf("motor_x = %d \n", motor_x_miss);
        pthread_mutex_unlock(&s16);
        
        pthread_mutex_lock(&s17);
        printf("motor_z = %d \n", motor_z_miss);
        pthread_mutex_unlock(&s17);
                
        pthread_mutex_lock(&s18);
        printf("adv_x = %d \n", adv_x_miss);
        pthread_mutex_unlock(&s18);
        
        pthread_mutex_lock(&s19);
        printf("adv_z = %d \n", adv_z_miss);
        pthread_mutex_unlock(&s19);
        
        pthread_mutex_lock(&s20);
        printf("display = %d \n", display_miss);
        pthread_mutex_unlock(&s20);
        
        pthread_mutex_lock(&s21);
        printf("tastiera = %d \n", tastiera_miss);
        pthread_mutex_unlock(&s21);

        wait_for_activation(i);

    }

}