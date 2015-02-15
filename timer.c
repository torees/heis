#include <time.h>
#include <stdio.h>
#include "timer.h"

static clock_t start,diff;

void t_timerStart(){
	start =clock();
}


int t_isTimeOut(){
	diff = clock() - start;
	int timesec = diff/CLOCKS_PER_SEC;
	if(timesec ==3)
		return 1;
	else 
		return 0;
}