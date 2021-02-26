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

void prediction(struct win* w, struct coord memory[DIM]){

    int i, r;

    memory[BEFORE].x = memory[NEXT].x;
    memory[BEFORE].z = memory[NEXT].z;

    while (!centroid(*w, &memory[NOW])){    //vede dove si trova la pallina adesso
        w->xsize += DELTA_X;
        w->zsize += DELTA_Z;
    }
    /* predizione lungo x */
    memory[NEXT].x = (2 * memory[NOW].x) - memory[BEFORE].x;
    /* predizione lungo y */
    memory[NEXT].z = (2 * memory[NOW].z) - memory[BEFORE].z;
    
    /* sposto la finestra di ricerca */
    w->x0 = memory[NEXT].x - (w->xsize / 2);
    w->z0 = memory[NEXT].z - (w->zsize / 2);
}

void* camera(void* arg){

    int             i;          //task index
    struct coord    temp[DIM]; 

        i = get_task_index(arg);
        set_activation(i);

        sem_wait(&s3);
        temp[BEFORE].x = buffer[BEFORE].x;
        temp[BEFORE].z = buffer[BEFORE].z;
        temp[NOW].x = buffer[NOW].x;
        temp[NOW].z = buffer[NOW].z;
        temp[NEXT].x = buffer[NEXT].x;
        temp[NEXT].z = buffer[NEXT].z;
        sem_post(&s3);

        //sem_wait(&s2);
        while(!end){
            
            //sem_wait(&s4);
            prediction(&window, temp);
            //sem_post(&s4);

            sem_wait(&s3);
            buffer[BEFORE].x = temp[BEFORE].x;
            buffer[BEFORE].z = temp[BEFORE].z;
            buffer[NOW].x = temp[NOW].x;
            buffer[NOW].z = temp[NOW].z;
            buffer[NEXT].x = temp[NEXT].x;
            buffer[NEXT].z = temp[NEXT].z;
            sem_post(&s3);

            if (deadline_miss(i))
                show_dmiss(i);
                
            wait_for_activation(i);
        }
        //sem_post(&s2);
}