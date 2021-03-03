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
    buffer[BEFORE].x = 320;
    buffer[BEFORE].z = 240;
    buffer[NOW].x = 320;
    buffer[NOW].z = 240;
    buffer[NEXT].x = 320;
    buffer[NEXT].z = 240;
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
                xc += (i - xc) / sample;    /* calcolo la media incrementale sia per x che per y */
                zc += (j - zc) / sample;
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
    if (trovato){
        w->xsize /= 3;
        w->zsize /= 3;
    }
    else
    {
        w->xsize = SIZE_X;
        w->zsize = SIZE_Z;
        w->x0 = 160;
        w->z0 = 60;
    }
    
    /* predizione lungo x */
    memory[NEXT].x = (2 * memory[NOW].x) - memory[BEFORE].x;
    /* predizione lungo z */
    memory[NEXT].z = (2 * memory[NOW].z) - memory[BEFORE].z;
    
    /* sposto la finestra di ricerca */
    //w->x0 = memory[NEXT].x - (w->xsize / 2);
    w->x0 = memory[NEXT].x;
    //w->z0 = memory[NEXT].z - (w->zsize / 2);
    w->z0 = memory[NEXT].z;
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