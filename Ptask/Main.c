#include "PTask.h"
#include <stdlib.h>
#include <stdio.h>


struct	sched_param	mypar;

int main(){

	for	(int i	=	0;	i	<	10;	i++){
		task_create(ball,	i,	50,	40,	30);
	}
}
