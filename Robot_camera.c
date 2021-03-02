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
