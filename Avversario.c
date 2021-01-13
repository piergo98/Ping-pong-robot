#include "Avversario.h"

int col = 14;       // yellow color

void* adversarytask(void* arg)
{
    install_keyboard();
    install_mouse();

        adversary_x.position = mouse_x;
        adversary_z.position = mouse_y;
    
}