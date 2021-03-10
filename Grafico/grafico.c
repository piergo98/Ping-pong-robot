#include    "motor.h"
#include    "grafico.h"

BITMAP* memory;         //bitmap provvisoria in cui ho una replica dell'interfaccia da disegnare

void init_screen(void){

    allegro_init();
    set_color_depth(COLOR);
    set_gfx_mode(GFX_AUTODETECT_WINDOWED, WIDTH, HEIGTH, 0, 0);
    clear_to_color(screen, BLACK);  //schermo nero
    install_keyboard();
    install_mouse();

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

    tempo = 0;
    pos_old = 240;
    vel_old = 0;

    memory = create_bitmap(WIDTH, HEIGTH);
    clear_bitmap(memory);
    line(memory, 0, 480 - TARGET, 640, 480 - TARGET, RED);
}

void draw_graph(BITMAP* buf){

        pthread_mutex_lock(&s6);
        line(buf, tempo, 480 - pos_old, tempo+1, 480 - robot_x.position, WHITE);
        pos_old = robot_x.position;
        line(buf, tempo, 480 - vel_old, tempo+1, 480 - robot_x.speed, BLUE);
        vel_old = robot_x.speed;
        pthread_mutex_unlock(&s6);
        tempo++;
        if (tempo > 639){
            clear_bitmap(buf);
            line(buf, 0, 480 - TARGET, 640, 480 - TARGET, RED);
            tempo = 0;
        }             
}

void* display(void* arg){

    int i;              //task index

        i = get_task_index(arg);
        set_activation(i);

        while(!end){
            draw_graph(memory);
            blit(memory, screen, 0, 0, 0, 0, WIDTH, HEIGTH);

            if (deadline_miss(i))
                show_dmiss(i);
            wait_for_activation(i);
        }
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
                    default: break; //da aggiungere altre opzioni

                }
               
                if (deadline_miss(i))
                    show_dmiss(i);

                wait_for_activation(i);
            } while(scan != KEY_ESC);
            //sem_wait(&s2);
            end = 1;
            //sem_post(&s2);

            if (deadline_miss(i))
                show_dmiss(i);

            wait_for_activation(i);
}