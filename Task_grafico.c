#include    <stdlib.h>
#include    <stdio.h>

#include    "Task_grafico.h"
#include    "Pallina.h"


char    punti_rob[DIM_S];
char    punti_avv[DIM_S];
int     p_rob;          //punteggio robot
int     p_avv;          //punteggio avversario
int     pview;          //indicatore per rappresentazione prospettica
BITMAP* rac;            //buffer per copiare lo schermo

void init_screen(void){

    init_camera();
    allegro_init();
    set_color_depth(COLOR);
    set_gfx_mode(GFX_AUTODETECT_WINDOWED, WIDTH, HEIGTH, 0, 0);
    clear_to_color(screen, BLACK);  //schermo nero
    install_keyboard();
    install_mouse();
    rac = load_bitmap("racchetta_nosfondo32.bmp", NULL);
    white2pink(rac);
    rac = load_bitmap("racp.bmp", NULL);

    //task_create(display, indice, periodo, drel, priorita);

}

void testo(BITMAP* buf){

    //crea un buffer per realizzare lo sfondo
    //buf = create_bitmap(WIDTH, HEIGTH);
    //blit(screen, buf, 0, 0, 0, 0, WIDTH, HEIGTH);

    textout_ex(buf, font, "PUNTEGGIO", P_X, P_Z, WHITE, TRASP);
    sprintf(punti_rob, "ROB = %d", p_rob);
    textout_ex(buf, font, punti_rob, P_X, P_Z + 20, WHITE, TRASP);  
    sprintf(punti_avv, "AVV = %d", p_avv);
    textout_ex(buf, font, punti_avv, P_X, P_Z + 40, WHITE, TRASP);
    textout_ex(buf, font, "LEGENDA:", X_LEG, P_Z, WHITE, TRASP);
    textout_ex(buf, font, "R -> Gioca robot", X_LEG, P_Z + 20, WHITE, TRASP);
    textout_ex(buf, font, "U -> Gioca utente", X_LEG, P_Z + 40, WHITE, TRASP);
    //textout_ex(screen, font, "V -> Vista verticale", X_LEG, P_Z + 60, WHITE, TRASP);
    textout_ex(buf, font, "SPACE -> Battuta", X_LEG, P_Z + 80, WHITE, TRASP);
    textout_ex(buf, font, "utente", X_LEG + 80, P_Z + 90, WHITE, TRASP);
}

void draw_screen(void){

    int i;
    int vertici[VERTEX] = {P1_X, P1_Z, P2_X, P2_Z, P3_X, P3_Z, P4_X, P4_Z};
    p_avv = 0;
    p_rob = 0;
    BITMAP* buf;

    //crea un buffer per realizzare lo sfondo
    buf = create_bitmap(WIDTH, HEIGTH);
    //blit(screen, buf, 0, 0, 0, 0, WIDTH, HEIGTH);


    // Disegna il tavolo
    polygon(buf, 4, vertici, FIELD);
    line(buf, 320, 420, 320, 100, WHITE);

    // Disegna la rete
    for (i = 0;i < 381;i += 5){
        line(buf, 130 + i, 240, 130 + i, 210, WHITE);
    }
    for (i = 0;i < 31;i += 5){
        line(buf, 130, 210 + i, 510, 210 + i, WHITE);
    }
    blit(buf, screen, 0, 0, 0, 0, WIDTH, HEIGTH);
    testo(buf);
    textout_ex(buf, font, "V -> Vista verticale", X_LEG, P_Z + 60, WHITE, TRASP);
    
    //trasferisco la bitmap sullo schermo
    blit(buf, screen, 0, 0, 0, 0, WIDTH, HEIGTH);
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

    int x, z, x_old, z_old;
    BITMAP* aux, *buf;

    aux = create_bitmap(WIDTH, HEIGTH);
    clear_bitmap(aux);

    if (pview)
    {
       prospective_view(adversary_x.position, Y_0, adversary_z.position);

       x = gcord.x;
       z = gcord.z;

    }

    else 
    {
        x = adversary_x.position;
        z = adversary_z.position;
    }
    
    x_old = x;
    z_old = z;

    while(!keypressed()){

        blit(screen, aux, 0, 0, 0, 0, WIDTH, HEIGTH);
        blit(screen, aux, x, z, x, z, w, h);
        stretch_sprite(screen, bmp, x, z, w, h);
    
        x_old = x;
        z_old = z;
        
        if (pview)
        {
            prospective_view(adversary_x.position, Y_0, adversary_z.position);

            x = gcord.x;
            z = gcord.z;

        }

        else 
        {
            x = adversary_x.position;
            z = adversary_z.position;
        }

        blit(aux, screen, x_old, z_old, x_old, z_old, w, h);
    }    
}

void racchetta_robot(BITMAP* bmp, int w, int h){

    int x, z, x_old, z_old;
    BITMAP* aux, *buf;

    aux = create_bitmap(WIDTH, HEIGTH);
    clear_bitmap(aux);

    if (pview)
    {
       prospective_view(robot_x.position, Y_0, robot_z.position);

       x = gcord.x;
       z = gcord.z;

    }

    else 
    {
        x = robot_x.position;
        z = robot_z.position;
    }
    
    x_old = x;
    z_old = z;

    while(!keypressed()){

        blit(screen, aux, 0, 0, 0, 0, WIDTH, HEIGTH);
        blit(screen, aux, x, z, x, z, w, h);
        stretch_sprite(screen, bmp, x, z, w, h);
    
        x_old = x;
        z_old = z;
        
        if (pview)
        {
            prospective_view(robot_x.position, Y_0, robot_z.position);

            x = gcord.x;
            z = gcord.z;

        }

        else 
        {
            x = robot_x.position;
            z = robot_z.position;
        }

        blit(aux, screen, x_old, z_old, x_old, z_old, w, h);
    }    
}

void draw_ball(void)
{
    int x, z;

    if (pview)
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

    draw_screen();
    while(1)
        //racchetta(rac, we, he);

    if (deadline_miss(i))
        //show_dmiss
    wait_for_activation(i);
}

void display_camera(){

    BITMAP* buf;

    //crea un buffer per realizzare lo sfondo
    buf = create_bitmap(WIDTH, HEIGTH);
    clear_bitmap(buf);

    //nasconde il cursore del mouse
    scare_mouse();
    // Disegna il tavolo visto dall'alto
    rectfill(buf, C_X1, C_Z1, C_X2, C_Z2, FIELD);
    rect(buf, C_X1, C_Z1, C_X2, C_Z2, WHITE);
    line(buf, 320, 420, 320, 60, WHITE);     // linea che divide il campo in due meta verticalmente
    line(buf, 140, 240, 500, 240, WHITE);    // linea della rete
    blit(buf, screen, 0, 0, 0, 0, WIDTH, HEIGTH);
    //testo(buf);
    textout_ex(buf, font, "V -> Indietro", X_LEG + 40, P_Z + 60, WHITE, TRASP);

    blit(buf, screen, 0, 0, 0, 0, WIDTH, HEIGTH);
    //draw_ball();
    unscare_mouse();
    
    //while (!keypressed()){      //da aggiustare il while
        /* Predice la posizione della pallina */
        //prediction(&window);
        /* Disegna la finestra di ricerca della pallina */
        //rect(buf, window.x0-(SIZE_X/2), window.z0+(SIZE_Z/2), window.x0+(SIZE_X/2), window.z0-(SIZE_Z/2), RED);
    
        //blit(buf, screen, 0, 0, 0, 0, WIDTH, HEIGTH);
    //} 


}

void prospective_view(int x, int y, int z)
{
    int x1, y1, z1;

    /* Rotazione su x di un angolo theta */
    x1 = x;
    y1 = COS_THETA * y - SIN_THETA * z;
    z1 = SIN_THETA * y + COS_THETA * z;

    /* Determinazione coordinate su piano prospettico */
    gcord.x = P2_X + x1 * POV_DIST / (POV_DIST - z1);
    gcord.z = P2_Z + y1 * POV_DIST / (POV_DIST - z1);
}