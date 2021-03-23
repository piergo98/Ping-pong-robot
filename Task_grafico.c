#include    <stdlib.h>
#include    <stdio.h>

#include    "Task_grafico.h"
#include    "Pallina.h"


char    punti_rob[DIM_S];
char    punti_avv[DIM_S];
char    string[DIM_S];

BITMAP* rac_r, *rac_a;  //buffer per copiare la racchetta
BITMAP* memory;         //bitmap provvisoria in cui ho una replica dell'interfaccia da disegnare

void init_screen(void){

    allegro_init();
    set_color_depth(COLOR_DEPTH);
    set_gfx_mode(GFX_AUTODETECT_WINDOWED, WIDTH, HEIGTH, 0, 0);
    clear_to_color(screen, BLACK);  //schermo nero
    install_keyboard();
    install_mouse();
    rac_r = load_bitmap("racchetta_nosfondo32.bmp", NULL);
    white2pink(rac_r);
    rac_r = load_bitmap("racp.bmp", NULL);
    rac_a = rac_r;

    pview_flag = 0;                  //valore di default

    gcord.x = 0;
    gcord.z = 0;

    pthread_mutex_lock(&s14);
    ball_miss = 0; 
    pthread_mutex_unlock(&s14);

    pthread_mutex_lock(&s15);
    camera_miss = 0;
    pthread_mutex_unlock(&s15);
                
    pthread_mutex_lock(&s16);
    motor_x_miss = 0;
    pthread_mutex_unlock(&s16);
                
    pthread_mutex_lock(&s17);
    motor_z_miss = 0;
    pthread_mutex_unlock(&s17);
            
    pthread_mutex_lock(&s18);
    adv_x_miss = 0;
    pthread_mutex_unlock(&s18);
                
    pthread_mutex_lock(&s19);
    adv_z_miss = 0;
    pthread_mutex_unlock(&s19);
                
    pthread_mutex_lock(&s20);
    display_miss = 0;
    pthread_mutex_unlock(&s20);
                
    pthread_mutex_lock(&s21);
    tastiera_miss = 0;
    pthread_mutex_unlock(&s21);

    pov = 600;
    angle = 0;

    memory = create_bitmap(WIDTH, HEIGTH);
    clear_bitmap(memory);
}

void testo(BITMAP* buf){ 

    /* Punteggio */
    textout_ex(buf, font, "PUNTEGGIO", P_X, P_Z, WHITE, TRASP);
    pthread_mutex_lock(&s1);
    sprintf(punti_rob, "ROB = %d", p_rob);
    pthread_mutex_unlock(&s1);
    textout_ex(buf, font, punti_rob, P_X, P_Z + 20, WHITE, TRASP);
    pthread_mutex_lock(&s2);  
    sprintf(punti_avv, "AVV = %d", p_avv);
    pthread_mutex_unlock(&s2);
    textout_ex(buf, font, punti_avv, P_X, P_Z + 40, WHITE, TRASP);
    /* Legenda */
    textout_ex(buf, font, "LEGENDA:", X_LEG, P_Z, WHITE, TRASP);
    textout_ex(buf, font, "R -> Gioca robot", X_LEG, P_Z + 20, WHITE, TRASP);
    textout_ex(buf, font, "U -> Gioca utente", X_LEG, P_Z + 40, WHITE, TRASP);
    textout_ex(buf, font, "SPACE -> Battuta", X_LEG, P_Z + 80, WHITE, TRASP);
    textout_ex(buf, font, "utente", X_LEG + 80, P_Z + 90, WHITE, TRASP); 
    
    /* Coordinate pallina e racchette 
    pthread_mutex_lock(&s13);
    sprintf(string, "Xp = %f", ball.x);
    pthread_mutex_unlock(&s13);
    textout_ex(buf, font, string, 500, 140, WHITE, TRASP);
    pthread_mutex_lock(&s13);
    sprintf(string, "Yp = %f", ball.y);
    pthread_mutex_unlock(&s13);
    textout_ex(buf, font, string, 500, 160, WHITE, TRASP);
    pthread_mutex_lock(&s13);
    sprintf(string, "Zp = %f", ball.z);
    pthread_mutex_unlock(&s13);
    textout_ex(buf, font, string, 500, 180, WHITE, TRASP);*/    

    /*pthread_mutex_lock(&s8);
    sprintf(string, "Xa = %d", adversary_x.position);
    pthread_mutex_unlock(&s8);
    textout_ex(buf, font, string, 500, 200, WHITE, TRASP);    

    pthread_mutex_lock(&s9);
    sprintf(string, "Za = %d", adversary_z.position);
    pthread_mutex_unlock(&s9);
    textout_ex(buf, font, string, 500, 220, WHITE, TRASP);

    pthread_mutex_lock(&s6);
    sprintf(string, "Xr = %d", robot_x.position);
    pthread_mutex_unlock(&s6);
    textout_ex(buf, font, string, 500, 240, WHITE, TRASP);

    pthread_mutex_lock(&s7);
    sprintf(string, "Zr = %d", robot_z.position);
    pthread_mutex_unlock(&s7);
    textout_ex(buf, font, string, 500, 260, WHITE, TRASP);

    pthread_mutex_lock(&s13);
    sprintf(string, "V_X = %f", ball.vx);
    pthread_mutex_unlock(&s13);
    textout_ex(buf, font, string, 500, 360, WHITE, TRASP);
    pthread_mutex_lock(&s13);
    sprintf(string, "V_Z = %f", ball.vz);
    pthread_mutex_unlock(&s13);
    textout_ex(buf, font, string, 500, 380, WHITE, TRASP);
    pthread_mutex_lock(&s3);
    sprintf(string, "R_X = %d", buffer[NOW].x);
    pthread_mutex_unlock(&s3);
    textout_ex(buf, font, string, 30, 400, WHITE, TRASP);
    pthread_mutex_lock(&s3);
    sprintf(string, "R_Z = %d", buffer[NOW].z);
    pthread_mutex_unlock(&s3);
    textout_ex(buf, font, string, 30, 420, WHITE, TRASP);
    pthread_mutex_lock(&s3);
    sprintf(string, "A_X = %d", buffer[NEXT].x);
    pthread_mutex_unlock(&s3);
    textout_ex(buf, font, string, 30, 440, WHITE, TRASP);
    pthread_mutex_lock(&s3);
    sprintf(string, "A_Z = %d", buffer[NEXT].z);
    pthread_mutex_unlock(&s3);
    textout_ex(buf, font, string, 30, 460, WHITE, TRASP);*/

}

void draw_screen(BITMAP* buf){

    int i;
    int vertici[VERTEX];
    int x_l1, z_l1, x_l2, z_l2;

        p_avv = 0;
        p_rob = 0;

        //Converto le coordinate del campo in prospettiva
        prospective_view(P1_X, 0, P1_Z);       //in basso a sinistra
        vertici[0] = gcord.x;
        vertici[1] = gcord.z;
        prospective_view(P2_X, 0, P2_Z);      //in alto a sinistra
        vertici[2] = gcord.x;
        vertici[3] = gcord.z;
        prospective_view(P3_X, 0, P3_Z);      //in alto a destra
        vertici[4] = gcord.x;
        vertici[5] = gcord.z;
        prospective_view(P4_X, 0, P4_Z);      //in basso a destra
        vertici[6] = gcord.x;
        vertici[7] = gcord.z;

        printf("x1 = %d\n", vertici[0]);
        printf("z1 = %d\n", vertici[1]);
        printf("x2 = %d\n", vertici[2]);
        printf("z2 = %d\n", vertici[3]);
        printf("x3 = %d\n", vertici[4]);
        printf("z3 = %d\n", vertici[5]);
        printf("x4 = %d\n", vertici[6]);
        printf("z4 = %d\n", vertici[7]);
        

        //crea un buffer per realizzare lo sfondo
        clear_bitmap(buf);

        // Disegna il tavolo
        polygon(buf, 4, vertici, FIELD);

        prospective_view(140, 0, 240);
        x_l1 = gcord.x;
        z_l1 = gcord.z;
        prospective_view(500, 0, 240);
        x_l2 = gcord.x;
        z_l2 = gcord.z; 

        line(buf, x_l1, z_l1, x_l2, z_l2, WHITE);
    
        //testo(buf);
        textout_ex(buf, font, "V -> Vista verticale", X_LEG, P_Z + 60, WHITE, TRASP);
}

void white2pink(BITMAP* b){

    BITMAP* racp;
    PALETTE pal;
    int     i, j, c;
    int     pink;
    
            pink = makecol(255, 0, 255);
            b = load_bitmap("racchetta_nosfondo32.bmp", NULL);
            racp = create_bitmap(b->w, b->h);
            for (i = 0; i < b->w; i++){
                for (j = 0; j < b->h; j++){
                    c = getpixel(b, i, j);
                    if (c == WHITE) c = pink;
                    putpixel(racp, i, j, c);
                }
            }
            get_palette(pal);
            save_bitmap("racp.bmp", racp, pal);
}

void racchetta_avversario(BITMAP* bmp, BITMAP* finestra, int w, int h){

    int x, z;

        if (pview_flag)
        {
            pthread_mutex_lock(&s8);
            pthread_mutex_lock(&s9);
            prospective_view(adversary_x.position, Y_0, adversary_z.position);
            pthread_mutex_unlock(&s9);
            pthread_mutex_unlock(&s8);

            x = gcord.x;
            z = gcord.z;
            //printf("racchetta_adv_Z = %d \t", z);
            //printf("racchetta_adv_X = %d \n", x);
            stretch_sprite(finestra, bmp, x, z, w, h);
            
        }
        else 
        {
            pthread_mutex_lock(&s8);
            x = adversary_x.position;
            pthread_mutex_unlock(&s8);
            pthread_mutex_lock(&s9);
            z = adversary_z.position;
            pthread_mutex_unlock(&s9);            

            rectfill(finestra, x - 20, z - 5, x + 20, z + 5, RED);
        } 
}

void racchetta_robot(BITMAP* bmp, BITMAP* finestra, int w, int h){

    int x, z, x_old, z_old;

        if (pview_flag)
        {
            pthread_mutex_lock(&s7);
            pthread_mutex_lock(&s6);
            prospective_view(robot_x.position, Y_0, robot_z.position);
            pthread_mutex_unlock(&s6);
            pthread_mutex_unlock(&s7);

            x = gcord.x;
            z = gcord.z;
            //printf("racchetta_rob_Z = %d \t", z);
            //printf("racchetta_rob_X = %d \n", x);
            stretch_sprite(finestra, bmp, x, z, w, h);
        }
        else 
        {
            pthread_mutex_lock(&s6);
            x = robot_x.position;
            pthread_mutex_unlock(&s6);
            pthread_mutex_lock(&s7);
            z = robot_z.position;
            pthread_mutex_unlock(&s7);
            rectfill(finestra, x - 20, z - 5, x + 20, z + 5, BLUE);
        } 
} 

void draw_ball(BITMAP* finestra)
{
    int x, z;
    struct status ball_local;

        pthread_mutex_lock(&s13);
        ball_local.x = ball.x;
        ball_local.y = ball.y;
        ball_local.z = ball.z;
        pthread_mutex_unlock(&s13);

        if (pview_flag)
        {
            prospective_view(ball_local.x, ball_local.y, ball_local.z);

            x = gcord.x;
            z = gcord.z;
            printf("ball_local_Z = %d \t", z);
            printf("ball_local_X = %d \n", x);
            circlefill(finestra, x, z, BALL_RADIUS, BALL_COLOR);
        }
        else 
        {
            x = ball_local.x;
            z = ball_local.z;
        }
    
        circlefill(finestra, x, z, BALL_RADIUS, BALL_COLOR);
}

void* display(void* arg){

    int i;              //task index
    int we = DIM_RAC;
    int he = DIM_RAC;

        i = get_task_index(arg);
        set_activation(i);

        while(!end){
           
            if (pview_flag)
                draw_screen(memory);
            else
                display_camera_view(memory);

            draw_ball(memory);            
            racchetta_avversario(rac_a, memory, we , he);
            racchetta_robot(rac_r, memory, we, he);

            blit(memory, screen, 0, 0, 0, 0, WIDTH, HEIGTH);

            if (deadline_miss(i))
                show_dmiss(i);
            wait_for_activation(i);
        }
}

void display_camera_view(BITMAP* buf){

    int vertici[VERTEX] = {P1_X, P1_Z, P2_X, P2_Z, P3_X, P3_Z, P4_X, P4_Z};

    // Disegna il tavolo visto dall'alto
    clear_bitmap(buf);
    polygon(buf, 4, vertici, FIELD);
    line(buf, 320, 420, 320, 60, WHITE);     // linea che divide il campo in due meta verticalmente
    line(buf, 140, 240, 500, 240, WHITE);    // linea della rete
    
    /* Disegna la finestra di ricerca della pallina */
    pthread_mutex_lock(&s4);
    rect(buf, window.x0, window.z0+window.zsize, window.x0+window.xsize, window.z0, RED);
    pthread_mutex_unlock(&s4);

    testo(buf);
    textout_ex(buf, font, "P -> Indietro", X_LEG + 40, P_Z + 60, WHITE, TRASP);
}

void prospective_view(int x, int y, int z)
{
    int x1, y1, z1, a, b, k;

        /* Rotazione su x di un angolo theta e su y di phi */
        x1 = x;                                         
        y1 = cos(angle) * y - sin(angle) * z;
        z1 = sin(angle) * y + cos(angle) * z;

        /* Determinazione coordinate su piano prospettico */
        //gcord.x = (660 + x1 * (POV_DIST / (- POV_DIST + z1)));
        //gcord.z = (100 - y1 * (POV_DIST / (-POV_DIST + z1)));

        if (z1 == pov) z1 = pov - 1;

        k = pov / (pov - z1);

        a = 160 + x1 * k;      //non so perche' se assegno direttamente a gcord non funge 
        b = 60 - y1 * k;      //stesso problema di su

        gcord.x = a;
        gcord.z = b;
}

void* command(void* arg){

    int     i;
    char    scan;

            i = get_task_index(arg);
            set_activation(i);

            do {
                scan = 0;
                if (keypressed()) scan = readkey() >> 8;
                switch(scan){
                    case KEY_V:
                        pview_flag = 0;
                        break;
                    case KEY_P:
                        pview_flag = 1;
                        break;
                    case KEY_U:
                        pthread_mutex_lock(&s10);
                        player = 1;
                        pthread_mutex_unlock(&s10);
                        break;
                    case KEY_R:
                        pthread_mutex_lock(&s10);
                        player = 0;
                        pthread_mutex_unlock(&s10);
                        break;
                    case KEY_SPACE:
                        pthread_mutex_lock(&s11);
                        start = 1;
                        pthread_mutex_unlock(&s11);
                        break;
                    case KEY_UP:
                        pov += 5;
                        break;
                    case KEY_DOWN:
                        pov -= 5;
                        break;
                    case KEY_RIGHT:
                        angle += 1;
                        break;
                    case KEY_LEFT:
                        angle -= 1;
                        break;
                    default: break; //da aggiungere altre opzioni

                }
               
                if (deadline_miss(i))
                    show_dmiss(i);

                wait_for_activation(i);
            } while(scan != KEY_ESC);
            
            end = 1;

            if (deadline_miss(i))
                show_dmiss(i);

            wait_for_activation(i);
}

