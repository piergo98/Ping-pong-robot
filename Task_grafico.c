#include    <stdlib.h>
#include    <stdio.h>

#include    "Task_grafico.h"
#include    "Pallina.h"


char    punti_rob[DIM_S];
char    punti_avv[DIM_S];
char    string[DIM_S];
int     p_rob;          //punteggio robot
int     p_avv;          //punteggio avversario

BITMAP* rac_r, *rac_a;  //buffer per copiare lo schermo
BITMAP* memory;         //bitmap provvisoria in cui ho una replica dell'interfaccia da disegnare

void init_screen(void){

    allegro_init();
    set_color_depth(COLOR);
    set_gfx_mode(GFX_AUTODETECT_WINDOWED, WIDTH, HEIGTH, 0, 0);
    clear_to_color(screen, BLACK);  //schermo nero
    install_keyboard();
    install_mouse();
    rac_r = load_bitmap("racchetta_nosfondo32.bmp", NULL);
    white2pink(rac_r);
    rac_r = load_bitmap("racp.bmp", NULL);
    rac_a = load_bitmap("racp.bmp", NULL);

    pview_flag = 0;                  //valore di default

    sem_wait(&s14);
    ball_miss = 0; 
    sem_post(&s14);

    sem_wait(&s15);
    camera_miss = 0;
    sem_post(&s15);
                
    sem_wait(&s16);
    motor_x_miss = 0;
    sem_post(&s16);
                
    sem_wait(&s17);
    motor_z_miss = 0;
    sem_post(&s17);
            
    sem_wait(&s18);
    adv_x_miss = 0;
    sem_post(&s18);
                
    sem_wait(&s19);
    adv_z_miss = 0;
    sem_post(&s19);
                
    sem_wait(&s20);
    display_miss = 0;
    sem_post(&s20);
                
    sem_wait(&s21);
    tastiera_miss = 0;
    sem_post(&s21);

    memory = create_bitmap(WIDTH, HEIGTH);
    clear_bitmap(memory);
}

void testo(BITMAP* buf){

    /* Punteggio */
    textout_ex(buf, font, "PUNTEGGIO", P_X, P_Z, WHITE, TRASP);
    sprintf(punti_rob, "ROB = %d", p_rob);
    textout_ex(buf, font, punti_rob, P_X, P_Z + 20, WHITE, TRASP);  
    sprintf(punti_avv, "AVV = %d", p_avv);
    textout_ex(buf, font, punti_avv, P_X, P_Z + 40, WHITE, TRASP);
     /* Legenda */
    textout_ex(buf, font, "LEGENDA:", X_LEG, P_Z, WHITE, TRASP);
    textout_ex(buf, font, "R -> Gioca robot", X_LEG, P_Z + 20, WHITE, TRASP);
    textout_ex(buf, font, "U -> Gioca utente", X_LEG, P_Z + 40, WHITE, TRASP);
    textout_ex(buf, font, "SPACE -> Battuta", X_LEG, P_Z + 80, WHITE, TRASP);
    textout_ex(buf, font, "utente", X_LEG + 80, P_Z + 90, WHITE, TRASP);

    /* Deadline misses */
    sem_wait(&s14);
    sprintf(string, "ball = %d", ball_miss);
    sem_post(&s14);
    textout_ex(buf, font, string, 160, 440, WHITE, TRASP);
    sem_wait(&s15);
    sprintf(string, "camera = %d", camera_miss);
    sem_post(&s15);
    textout_ex(buf, font, string, 160, 460, WHITE, TRASP);            
    sem_wait(&s16);
    sprintf(string, "motor_x = %d", motor_x_miss);
    sem_post(&s16);
    textout_ex(buf, font, string, 260, 440, WHITE, TRASP);            
    sem_wait(&s17);
    sprintf(string, "motor_z = %d", motor_z_miss);
    sem_post(&s17);
    textout_ex(buf, font, string, 260, 460, WHITE, TRASP);        
    sem_wait(&s18);
    sprintf(string, "adv_x = %d", adv_x_miss);
    sem_post(&s18);
    textout_ex(buf, font, string, 360, 440, WHITE, TRASP);            
    sem_wait(&s19);
    sprintf(string, "adv_z = %d", adv_z_miss);
    sem_post(&s19);
    textout_ex(buf, font, string, 360, 460, WHITE, TRASP);            
    sem_wait(&s20);
    sprintf(string, "display = %d", display_miss);
    sem_post(&s20);
    textout_ex(buf, font, string, 460, 440, WHITE, TRASP);            
    sem_wait(&s21);
    sprintf(string, "tastiera = %d", tastiera_miss);
    sem_post(&s21);
    textout_ex(buf, font, string, 460, 460, WHITE, TRASP);   
    
    /* Coordinate pallina e racchette */
    sprintf(string, "Xp = %f", ball.x);
    textout_ex(buf, font, string, 500, 140, WHITE, TRASP);
    sprintf(string, "Yp = %f", ball.y);
    textout_ex(buf, font, string, 500, 160, WHITE, TRASP);
    sprintf(string, "Zp = %f", ball.z);
    textout_ex(buf, font, string, 500, 180, WHITE, TRASP);
    sprintf(string, "Xa = %d", adversary_x.position);
    textout_ex(buf, font, string, 500, 200, WHITE, TRASP);
    sprintf(string, "Za = %d", adversary_z.position);
    textout_ex(buf, font, string, 500, 220, WHITE, TRASP);
    sprintf(string, "Xr = %d", robot_x.position);
    textout_ex(buf, font, string, 500, 240, WHITE, TRASP);
    sprintf(string, "Zr = %d", robot_z.position);
    textout_ex(buf, font, string, 500, 260, WHITE, TRASP);
    sprintf(string, "Xc = %d", window.x0);
    textout_ex(buf, font, string, 500, 280, WHITE, TRASP);
    sprintf(string, "Zc = %d", window.z0);
    textout_ex(buf, font, string, 500, 300, WHITE, TRASP);

}

void draw_screen(BITMAP* buf){

    int i;
    int vertici[VERTEX] = {P1_X, P1_Z, P2_X, P2_Z, P3_X, P3_Z, P4_X, P4_Z};
        p_avv = 0;
        p_rob = 0;

        //crea un buffer per realizzare lo sfondo
        clear_bitmap(buf);

        // Disegna il tavolo
        polygon(buf, 4, vertici, FIELD);
        line(buf, 320, 420, 320, 100, WHITE);

        // Disegna la rete
        for (i = 0;i < 381;i += 5)
        line(buf, 130 + i, 240, 130 + i, 210, WHITE);

        for (i = 0;i < 31;i += 5)
            line(buf, 130, 210 + i, 510, 210 + i, WHITE);
    
        testo(buf);
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

void racchetta_avversario(BITMAP* bmp, int w, int h){

    int x, z;

        if (pview_flag)
        {
            prospective_view(adversary_x.position, Y_0, adversary_z.position);

            x = gcord.x;
            z = gcord.z;
            stretch_sprite(screen, bmp, x, z, w, h);
        }
        else 
        {
            x = adversary_x.position;
            z = adversary_z.position;
            rectfill(screen, x - 20, z - 20, x + 20, z + 20, RED);
        } 
}

void racchetta_robot(BITMAP* bmp, int w, int h){

    int x, z, x_old, z_old;

        if (pview_flag)
        {
            prospective_view(robot_x.position, Y_0, robot_z.position);

            x = gcord.x;
            z = gcord.z;
            stretch_sprite(screen, bmp, x, z, w, h);
        }
        else 
        {
            x = robot_x.position;
            z = robot_z.position;
            rectfill(screen, x - 20, z - 20, x + 20, z + 20, RED);
        } 
} 

void draw_ball(void)
{
    int x, z;

        if (pview_flag)
        {
            prospective_view(ball.x, ball.y, ball.z);

            x = gcord.x;
            z = gcord.z;
        }
        else 
        {
            x = ball.x;
            z = ball.z;
        }
    
        circlefill(screen, x, z, BALL_RADIUS, BALL_COLOR);
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
            blit(memory, screen, 0, 0, 0, 0, WIDTH, HEIGTH);

            draw_ball();
        
            racchetta_avversario(rac_a, we , he);
            racchetta_robot(rac_r, we, he);

            /* Disegna la finestra di ricerca della pallina */
            if (!pview_flag)
                rect(screen, window.x0-(SIZE_X/2), window.z0+(SIZE_Z/2), window.x0+(SIZE_X/2), window.z0-(SIZE_Z/2), RED);

            if (deadline_miss(i))
                show_dmiss(i);
            wait_for_activation(i);
    }
}

void display_camera_view(BITMAP* buf){

    // Disegna il tavolo visto dall'alto
    clear_bitmap(buf);
    rectfill(buf, C_X1, C_Z1, C_X2, C_Z2, FIELD);
    rect(buf, C_X1, C_Z1, C_X2, C_Z2, WHITE);
    line(buf, 320, 420, 320, 60, WHITE);     // linea che divide il campo in due meta verticalmente
    line(buf, 140, 240, 500, 240, WHITE);    // linea della rete

    testo(buf);
    textout_ex(buf, font, "P -> Indietro", X_LEG + 40, P_Z + 60, WHITE, TRASP);
}

void prospective_view(int x, int y, int z)
{
    int x1, y1, z1;

        /* Rotazione su x di un angolo theta */
        x1 = x;
        y1 = COS_THETA * y - SIN_THETA * z;
        z1 = SIN_THETA * y + COS_THETA * z;

        /* Determinazione coordinate su piano prospettico */
        gcord.x = -x1 * POV_DIST / (POV_DIST - z1);
        gcord.z = y1 * POV_DIST / (POV_DIST - z1);
}

void* command(void* arg){

    int     i;
    char    scan;

            i = get_task_index(arg);

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
                        player = 1;
                        break;
                    case KEY_R:
                        player = 0;
                        break;
                    default: break; //da aggiungere altre opzioni
                }
            } while(scan != KEY_ESC);
            sem_wait(&s2);
            end = 1;
            sem_post(&s2);

            if (deadline_miss(i))
                show_dmiss(i);

            wait_for_activation(i);
}

