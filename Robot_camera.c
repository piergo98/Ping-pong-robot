#include    "Robot_camera.h"

int centroid(struct win w, struct coord *target){

    int c, i, j;
    int sample, xc, yc;
    sample = xc = yc = 0;

    for (i = 0; i < w.xsize; i++){
        for (j = 0; j < w.ysize; j++){
            c = getpixel(screen, i + w.x0, j + w.y0);
            if (c == TARGET_COLOR){
                sample++;
                xc += (i - xc) / sample;    /* calcolo la media incrementale sia per x che per y */
                yc += (j - yc) / sample;
            }
        }
    }
    if (sample > THRESHOLD){            //target trovato
            target->x = xc;
            target->y = yc;
            return 1;
        }
        return 0;                       //target non trovato    
}

void prediction(struct win* w){

    int i, r;

    buffer[BEFORE].x = buffer[NEXT].x;
    buffer[BEFORE].y = buffer[NEXT].y;

    while (!centroid(*w, buffer)){
        w->xsize += INCREASE;
        w->ysize += INCREASE;
    }
    /* predizione lungo x */
    buffer[NEXT].x = (2 * buffer[NOW].x) - buffer[BEFORE].x;
    /* predizione lungo y */
    buffer[NEXT].y = (2 * buffer[NOW].y) - buffer[BEFORE].y;
    
    /* sposto la finestra di ricerca */
    w->x0 = buffer[NEXT].x - (w->xsize / 2);
    w->y0 = buffer[NEXT].y - (w->ysize / 2);
}

void* camera(void* arg){

    int i;      //task index
    
    /* Inizializzo la struttura puntata da window */
    window->x0 = 0;
    window->y0 = 0;
    window->xsize = SIZE_X;
    window->ysize = SIZE_Y;

    i = get_task_index(arg);

    while(!end){

        prediction(window);
        wait_for_activation(i);
    }
}