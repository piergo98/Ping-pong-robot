#include    "Robot_camera.h"

void init_camera(){

    /* Inizializzo la struttura puntata da window (DA METTERE NELLA FUNZIONE INIT()*/
    sem_wait(&s4);
    window.x0 = 320;
    window.z0 = 240;
    window.xsize = SIZE_X;
    window.zsize = SIZE_Z;
    sem_post(&s4);

    sem_wait(&s3);
    buffer[BEFORE].x = 0;
    buffer[BEFORE].z = 0;
    buffer[NOW].x = 0;
    buffer[NOW].z = 0;
    buffer[NEXT].x = 0;
    buffer[NEXT].z = 0;
    sem_post(&s3);
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

void prediction(struct win* w){

    int i, r;

    sem_wait(&s3);

    buffer[BEFORE].x = buffer[NEXT].x;
    buffer[BEFORE].z = buffer[NEXT].z;
    
    while (!centroid(*w, &buffer[NOW])){    //vede dove si trova la pallina adesso
        w->xsize += DELTA_X;
        w->zsize += DELTA_Z;
    }
    /* predizione lungo x */
    buffer[NEXT].x = (2 * buffer[NOW].x) - buffer[BEFORE].x;
    /* predizione lungo y */
    buffer[NEXT].z = (2 * buffer[NOW].z) - buffer[BEFORE].z;
    
    /* sposto la finestra di ricerca */
    w->x0 = buffer[NEXT].x - (w->xsize / 2);
    w->z0 = buffer[NEXT].z - (w->zsize / 2);

    sem_post(&s3);
}

void* camera(void* arg){

    int i;      //task index

        i = get_task_index(arg);
        set_activation(i);

        while(!end){

            sem_wait(&s4);
            prediction(&window);
            sem_post(&s4);

            if (deadline_miss(i))
                show_dmiss(i);
                
            wait_for_activation(i);
        }
}