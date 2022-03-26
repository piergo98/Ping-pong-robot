#include    <stdlib.h>
#include    <stdio.h>

#include    "Task_grafico.h"
#include    "Pallina.h"


char    punti_rob[DIM_S];
char    punti_avv[DIM_S];
char    string[DIM_S];

BITMAP* rac_r, *rac_a;  //buffer per copiare la racchetta
BITMAP* memory;         //bitmap provvisoria in cui disegno l'area di gioco e la legenda
BITMAP* mem;            //bitmap provvisoria in cui disegno i grafici

void init_screen(void){

    /* Inizializzo allegro e la finestra grafica */
    allegro_init();
    set_color_depth(COLOR_DEPTH);
    set_gfx_mode(GFX_AUTODETECT_WINDOWED, WIDTH, HEIGHT, 0, 0);
    clear_to_color(screen, BLACK);  //schermo nero
    install_keyboard();
    install_mouse();

    /* Carica la bitmap della racchetta (per prospettiva) */
    //rac_r = load_bitmap("racchetta_nosfondo32.bmp", NULL);
    //white2pink(rac_r);
    //rac_r = load_bitmap("racp.bmp", NULL);
    //rac_a = rac_r;

    /* Inizializza variabili prospettiva */
    pview_flag = 0;                  //valore di default

    gcord.x = 0;
    gcord.z = 0;
    pov = POV_DIST;
    angle = 0;

    /* Inizializza i contatori delle deadline miss */
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

    /* Inizializza variabili per grafici */
    tempo = TEMPO_INIT;
    pos_old_1 = H_GRAPH_1;
    pos_old_2 = H_GRAPH_2;
    pos_old_3 = H_GRAPH_3;
    pos_old_4 = H_GRAPH_4;

    /* Inizializzo la difficoltà di gioco*/
    level = 0;

    /* Inizializza bitmap provvisorie su cui vengono disegnati il campo e i grafici */
    memory = create_bitmap(WIDTH_GAME, HEIGHT_GAME);
    mem = create_bitmap(WIDTH_GRAPH, HEIGHT_GRAPH);
    clear_bitmap(memory);
    clear_bitmap(mem);
}

void legenda(BITMAP* buf){ 

    /* Disegno il rettangolo che contiene tutti i dati*/
    rect(buf, 0, DELTA, WIDTH_GAME, 0, WHITE);
    /* Punteggio */
    textout_ex(buf, font, "PUNTEGGIO", P_X, P_Z, WHITE, TRASP);
    pthread_mutex_lock(&s1);
    sprintf(punti_rob, "ROB = %d", p_rob);
    pthread_mutex_unlock(&s1);
    textout_ex(buf, font, punti_rob, P_X, P_Z + Z_DIST, WHITE, TRASP);
    pthread_mutex_lock(&s2);  
    sprintf(punti_avv, "AVV = %d", p_avv);
    pthread_mutex_unlock(&s2);
    textout_ex(buf, font, punti_avv, P_X, P_Z + 2 * Z_DIST, WHITE, TRASP);
    /* Legenda */
    textout_ex(buf, font, "LEGENDA:", X_LEG, P_Z, WHITE, TRASP);
    textout_ex(buf, font, "R -> Gioca robot", X_LEG, P_Z + Z_DIST, WHITE, TRASP);
    textout_ex(buf, font, "U -> Gioca utente", X_LEG, P_Z + 2 * Z_DIST, WHITE, TRASP);
    textout_ex(buf, font, "SPACE -> Battuta", X_LEG, P_Z + 3 * Z_DIST, WHITE, TRASP);
    textout_ex(buf, font, "SU -> Aumenta", X_DIF, P_Z + Z_DIST, WHITE, TRASP);
    textout_ex(buf, font, "difficoltà", X_DIF + X_DIST, P_Z + 1.5 * Z_DIST, WHITE, TRASP);
    textout_ex(buf, font, "GIÙ -> Riduci", X_DIF, P_Z + 2 * Z_DIST, WHITE, TRASP);
    textout_ex(buf, font, "difficoltà", X_DIF + X_DIST, P_Z + 2.5 * Z_DIST, WHITE, TRASP);
    /* Livello */
    textout_ex(buf, font, "DIFFICOLTÀ:", X_LEV, P_Z, WHITE, TRASP);
    if (level <= FACILE) {                                                                                  //facile
        textout_ex(buf, font, "Facile", X_LEV, P_Z + Z_DIST, WHITE, TRASP);
        level = FACILE;  //protezione per non far diventare level negativo
    }             
    else if (level == MEDIA) textout_ex(buf, font, "Media", X_LEV, P_Z + Z_DIST, WHITE, TRASP);             //media
    else if (level == DIFFICILE) textout_ex(buf, font, "Difficile", X_LEV, P_Z + Z_DIST, WHITE, TRASP);     //difficile
    else if (level >= ESTREMA) {                                                                            //estrema
        textout_ex(buf, font, "Estrema", X_LEV, P_Z + Z_DIST, WHITE, TRASP);
        level = ESTREMA;  //protezione per non far diventare troppo grande level
    }                    
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
        prospective_view(P2_X + (pov / 20), 0, P2_Z);      //in alto a sinistra
        vertici[2] = gcord.x;
        vertici[3] = gcord.z;
        prospective_view(P3_X - (pov / 20), 0, P3_Z);      //in alto a destra
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

        prospective_view(140 + (pov /25), 0, 240 - (pov / 50));
        x_l1 = gcord.x;
        z_l1 = gcord.z;
        prospective_view(500 - (pov / 25), 0, 240 - (pov / 50));
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

            rectfill(finestra, x - RACC_MIN, z - D, x + RACC_MAX, z + D, RED);
        } 
}

void racchetta_robot(BITMAP* bmp, BITMAP* finestra, int w, int h){

    int x, z, x_old, z_old, temp_x;

        if (pview_flag)
        {
            pthread_mutex_lock(&s7);
            pthread_mutex_lock(&s6);
            //temp_x = (robot_x.position - P2_X)*(1 - (2 * (pov / 20) / (P3_X - P2_X))) + P2_X + (pov / 20);
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
            rectfill(finestra, x - RACC_MIN, z - D, x + RACC_MAX, z + D, BLUE);
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

        i = get_task_index(arg);
        set_activation(i);

        while(!end){
           
            display_camera_view(memory);

            draw_ball(memory);            
            racchetta_avversario(rac_a, memory, DIM_RAC , DIM_RAC);
            racchetta_robot(rac_r, memory, DIM_RAC, DIM_RAC);
            draw_graph(mem);

            blit(memory, screen, 0, 0, 0, 0, WIDTH_GAME, HEIGHT_GAME);
            blit(mem, screen, 0, 0, WIDTH_GAME, 0, WIDTH_GRAPH, HEIGHT_GRAPH);

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
    line(buf, HALF_X, Z1_V, HALF_X, Z2_V, WHITE);       // linea che divide il campo in due meta verticalmente
    line(buf, X1_O, HALF_Z, X2_O, HALF_Z, WHITE);       // linea della rete
    rect(buf, 0, HEIGHT, WIDTH_GAME, DELTA, WHITE);
    
    /* Disegna la finestra di ricerca della pallina */
    pthread_mutex_lock(&s4);
    rect(buf, window.x0, window.z0+window.zsize, window.x0+window.xsize, window.z0, RED);
    pthread_mutex_unlock(&s4);

    legenda(buf);
}

void prospective_view(int x, int y, int z)
{
    int x1, y1, z1, a, b, k;
    float angle_rad;

        angle_rad  = angle * 3.14;

        printf("Angle = %f * pi \t", angle);
        printf("PoV = %d \n", pov);

        /* Rotazione su x di un angolo theta e su y di phi */
        x1 = x;                                         
        y1 = cos(angle_rad) * y - sin(angle_rad) * z;
        z1 = sin(angle_rad) * y + cos(angle_rad) * z;

        /* Determinazione coordinate su piano prospettico */
        //gcord.x = (660 + x1 * (POV_DIST / (- POV_DIST + z1)));
        //gcord.z = (100 - y1 * (POV_DIST / (-POV_DIST + z1)));

        if (z1 == pov) z1 = pov - 1;

        k = pov / (pov - z1);

        a = P1_X + x1 * k;      //non so perche' se assegno direttamente a gcord non funge 
        b = P1_Z - y1 * k;      //stesso problema di su

        gcord.x = a;
        gcord.z = b;
}

void* command(void* arg){

    int     i;
    float   vel;
    char    scan;

            i = get_task_index(arg);
            set_activation(i);

            do {
                scan = 0;
                if (keypressed()) scan = readkey() >> 8;
                switch(scan){
                    case KEY_V:
                        //pview_flag = 0;
                        break;
                    case KEY_P:
                        //pview_flag = 1;
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
                        vel = difficulty();
                        pthread_mutex_lock(&s13);
                        ball.vz = vel;
                        pthread_mutex_unlock(&s13);
                        break;
                    case KEY_UP:
                        level += 1;
                        break;
                    case KEY_DOWN:
                        level -= 1;
                        break;
                    case KEY_RIGHT:
                        //angle += 0.01;
                        break;
                    case KEY_LEFT:
                        //angle -= 0.01;
                        break;
                    default: break;

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

void draw_graph(BITMAP* buf){

    int pos_1, pos_2, pos_3, pos_4;

        pthread_mutex_lock(&s6);
        pos_1 = H_GRAPH_1 - (robot_x.position / SCALE);
        pthread_mutex_unlock(&s6);

        pthread_mutex_lock(&s7);
        pos_2 = H_GRAPH_2 - (robot_z.position / SCALE);
        pthread_mutex_unlock(&s7);

        pthread_mutex_lock(&s8);
        pos_3 = H_GRAPH_3 - (adversary_x.position / SCALE);
        pthread_mutex_unlock(&s8);

        pthread_mutex_lock(&s9);
        pos_4 = H_GRAPH_4 - (adversary_z.position / SCALE);
        pthread_mutex_unlock(&s9);

        /* Disegno l'area dei grafici */
        rect(buf, 0, H_GRAPH_1, WIDTH_GRAPH, 0, WHITE);
        textout_ex(buf, font, "Robot_X", X_GRAPH, Z_GRAPH_1, BLUE, TRASP);
        rect(buf, 0, H_GRAPH_2, WIDTH_GRAPH, H_GRAPH_1, WHITE);
        textout_ex(buf, font, "Robot_Z", X_GRAPH, Z_GRAPH_2, RED, TRASP);
        rect(buf, 0, H_GRAPH_3, WIDTH_GRAPH, H_GRAPH_2, WHITE);
        textout_ex(buf, font, "Avversario_X", X_GRAPH, Z_GRAPH_3, GREEN, TRASP);
        rect(buf, 0, H_GRAPH_4, WIDTH_GRAPH, H_GRAPH_3, WHITE);
        textout_ex(buf, font, "Avversario_Z", X_GRAPH, Z_GRAPH_4, FUCSIA, TRASP);
        /* Traccio i grafici */
        line(buf, tempo, pos_old_1, tempo+1, pos_1, BLUE);
        pos_old_1 = pos_1;
        line(buf, tempo, pos_old_2, tempo+1, pos_2, RED);
        pos_old_2 = pos_2;
        line(buf, tempo, pos_old_3, tempo+1, pos_3, GREEN);
        pos_old_3 = pos_3;
        line(buf, tempo, pos_old_4, tempo+1, pos_4, FUCSIA);
        pos_old_4 = pos_4;
        
        tempo++;
        if (tempo > TEMPO_MAX){
            clear_bitmap(buf);
            tempo = TEMPO_INIT;
        }             
}

float difficulty(){

    if (level <= FACILE) return VZ_FACILE;                          //facile
    else if (level == MEDIA) return VZ_MEDIO;                       //media
    else if (level == DIFFICILE) return VZ_DIFFICILE;               //difficile
    else if (level >= ESTREMA)  return VZ_ESTREMA;                  //estrema
}