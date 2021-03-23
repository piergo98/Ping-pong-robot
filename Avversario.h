#include    <stdlib.h>
#include    <stdio.h>
#include    <math.h>
#include    <allegro.h>
#include    <pthread.h>
#include    <time.h>

#include    "taskmotor.h"


int via_adv;


void* adversarytask_x(void* arg);

void* adversarytask_z(void* arg);

void update_adversary_state_x(float y, int T, int p_min, int p_max, struct state *robot_tmp);

void update_adversary_state_z(float y, int T, int p_min, int p_max, struct state *robot_tmp);
