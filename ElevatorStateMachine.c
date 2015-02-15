
#include <stdio.h>

#include "ElevatorStateMachine.h"
#include "elev.h"
#include "queue.h"
#include "timer.h"


EState g_state = state_idle;

int fsm_emergencyStop = 0;
static int targetFloor;
/*void fsm_setState(EState state){
    switch(state){
        case state_idle:
            g_state=state_idle;
            elev_set_stop_lamp(1);
            break; 

        case state_run:
            g_state=state_run;
            break; 

        case state_doorOpen:
            g_state=state_doorOpen;
            break; 

        case state_emergencyStopValidFloor:
            g_state=state_emergencyStopValidFloor;
            break; 

        case state_emergencyStopInvalidFloor:
            g_state=state_emergencyStopInvalidFloor;
            break; 

        case state_invalidFloorIdle:
            g_state=state_invalidFloorIdle;
            break; 

        default:
            printf("invalid state called");
            break;   

    }

}
*/

void fsm_setEmergencyStop(){
      
        
    q_deleteQueue();
    elev_set_motor_direction(DIRN_STOP); 
    fsm_emergencyStop = 1;

    switch (g_state) {
        case state_idle:
            g_state = state_emergencyStopValidFloor;
            elev_set_door_open_lamp(1);
            break;
            
        case state_doorOpen:
            g_state = state_emergencyStopValidFloor;
            elev_set_door_open_lamp(1);
            break;
            
        case state_run:
            g_state = state_emergencyStopInvalidFloor;
            break;

        case state_emergencyStopInvalidFloor:
            g_state=state_invalidFloorIdle;
            break;

        case state_invalidFloorIdle:
            g_state=state_emergencyStopInvalidFloor;
            break;
        case state_emergencyStopValidFloor:
            g_state=state_idle;
            elev_set_door_open_lamp(0);
            
        default:
            break;
    }   
    
}

// returnerer -1 ved tom kø
int fsm_getNextInQueue(){
    
    int i;
    if (g_state == state_doorOpen) {
        
        if (priorityDirection==1){
            
            for (i=currentFloor+1; i<4; i++) {
                if ((upQueue[i] != 0) || (commandQueue[i] != 0)) {
                    return i;
                }
            }
            
            //unntakstilfelle ved endestopp
            if (downQueue[3] != 0) {
                return 3;
            }
        }
        
        else if (priorityDirection == -1){
            
            for (i=currentFloor-1; i>-1; i--) {
                if ((downQueue[i] != 0) || (commandQueue[i] != 0)) {
                    return i;
                }
            }
            
            //unntakstilfelle ved endestopp
            if (upQueue[0] != 0) {
                return 0;
            }
        }
    }
    
    else if (g_state == state_idle){
        for (i=0; i<4; i++) {
            if ((upQueue[i] != 0) || (commandQueue[i] != 0) || (downQueue[i] != 0)) {
                return i;
            }
        }
    }        
    //if nothing, return -1
    return -1; 
}


void fsm_setPriorityDirection(int direction){
    elev_set_motor_direction(direction);
    if (direction != 0) {
        priorityDirection = direction;
    }
    
    if ((elev_get_floor_sensor_signal()==0 )||(elev_get_floor_sensor_signal()==3)) {
        priorityDirection = priorityDirection*(-1);
    }
}

void fsm_getOrders(){
    int i,j;
    for (i = 0; i <N_FLOORS; i++){
            for ( j=0;j<3;j++){
                //sjekker for manglende knapper i endeetasjer
                if ((i== 0 && j == BUTTON_CALL_DOWN) || (i==3 && j==BUTTON_CALL_UP)){} 
                //legger inn i kø
                else {                  
                    if (elev_get_button_signal(j,i)){
                        switch(j){
                            case BUTTON_CALL_DOWN:
                                q_addToQueue(-1,i);
                                break;
                            case BUTTON_CALL_UP:
                                q_addToQueue(1,i);
                                break;
                            case BUTTON_COMMAND:
                                q_addToQueue(2,i);
                                break;
                            default:
                                printf("Error in adding to queue");
                        }
                    }
                }
            }
        }                

}

void fsm_stateMachine(){
    //sjekk om nødstopp, gjelder for alle tilstander
    if(elev_get_stop_signal())
        fsm_setEmergencyStop();                
            
    

    switch (g_state){

        case state_idle:
            targetFloor=fsm_getNextInQueue();            
            
            if (targetFloor != -1){
                if(elev_get_floor_sensor_signal()==targetFloor){
                    g_state=state_doorOpen;
                    t_timerStart();
                    elev_set_door_open_lamp(1);
                    break;
                }
                //sjekker hvilke retning heisen må settes til
                else if (elev_get_floor_sensor_signal() < targetFloor)
                    priorityDirection=1;
                else if(elev_get_floor_sensor_signal() > targetFloor)
                    priorityDirection=-1;

                elev_set_motor_direction(priorityDirection);
                g_state = state_run;
                

            }
            break;

        case state_run:
            if(elev_get_floor_sensor_signal()==targetFloor){
                g_state=state_doorOpen;
                t_timerStart();
                elev_set_door_open_lamp(1);
            }
            break;

        case state_doorOpen:
            if(t_isTimeOut()){
                //invertering av priorityDirection-attributten
                if(elev_get_floor_sensor_signal()==0)
                    priorityDirection=1;
                else if (elev_get_floor_sensor_signal()==3)
                    priorityDirection=-1;


                elev_set_door_open_lamp(0);
                targetFloor=fsm_getNextInQueue();       

                if (targetFloor == -1)
                    g_state=state_idle;
                else
                    g_state=state_run;
                    //ved feil, sjekk om priority direction fungerer i dette i hht heisalgoritmen
                    elev_set_motor_direction(priorityDirection);

            }
            break;

        case state_invalidFloorIdle:

            targetFloor=fsm_getNextInQueue();
            if (elev_get_floor_sensor_signal() < targetFloor)
                    priorityDirection=1;
            else if(elev_get_floor_sensor_signal() > targetFloor)
                    priorityDirection=-1;

            elev_set_motor_direction(priorityDirection);
            g_state = state_run;
            break;          

        default:
            break;
    }
}