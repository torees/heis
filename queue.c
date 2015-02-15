#include <stdio.h>

#include "queue.h"


int downQueue[]={0,0,0,0};
int upQueue[]={0,0,0,0};
int commandQueue[]={0,0,0,0};

void q_addToQueue(int direction, int floor){

	if (direction == 1){
		upQueue[floor] = 1;

	}
	
	else if (direction == -1){
		downQueue[floor] = 1;
	}
	else if(direction == 2){
		commandQueue[floor]=1;
	}
	

	else {
	printf("Error in q_addToQueue()\n");
	}
}


void q_removeFromQueue(int direction, int floor){

	if (direction == 1){
		upQueue[floor] = 0;
	}
	
	else if (direction == -1){
		downQueue[floor] = 0;
	}

	else {
	printf("Error in q_removeFromQueue()\n");
	}
}

void q_printQueue(){
	

	printf("%d %d %d %d \n",upQueue[0],upQueue[1],upQueue[2],upQueue[3]);		
	
	printf("%d %d %d %d \n",downQueue[0],downQueue[1],downQueue[2],downQueue[3]);

	printf("%d %d %d %d \n",commandQueue[0],commandQueue[1],commandQueue[2],commandQueue[3]);
	
}

void q_deleteQueue(){
	
	int i;
	for (i=0; i<4; i++){
		upQueue[i] = 0;
		downQueue[i] = 0;
		commandQueue[i] = 0;
	}
}
