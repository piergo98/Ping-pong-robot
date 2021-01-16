#include "Avversario.h"

int col = 14;       // yellow color

void* adversarytask(void* arg)
{
    int i;

    i = get_task_index(arg);
    install_keyboard();
    install_mouse();
    
    /* Controlla e aggiorna x avversario */
    
    if (adversary_x.position >= C_X4 + OFFSET_X) 
        adversary_x.position = C_X4 + OFFSET_X;
    
    else if (adversary_x.position <= C_X1 - OFFSET_X) 
        adversary_x.position = C_X1 - OFFSET_X;
    
    else   
        adversary_x.position = mouse_x;

    /* Controlla e aggiorna z avversario */
    
    if (adversary_z.position >= C_Z1 + OFFSET_Z / 3) 
        adversary_z.position = C_Z1 + OFFSET_Z / 3;
    
    else if (adversary_z.position >= C_Z1 - OFFSET_Z) 
        adversary_x.position = C_X1 - OFFSET_X;
    
    else   
        adversary_z.position = mouse_y;

    wait_for_activation(i);        
}