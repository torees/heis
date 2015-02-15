#include "elev.h"
#include <stdio.h>
#include "queue.h"
#include "ElevatorStateMachine.h"

int main() {
	
    


    // Initialize hardware
    if (!elev_init()) {
        printf("Unable to initialize elevator hardware!\n");
        return 1;
    }

    printf("Press STOP button to stop elevator and exit program.\n");



    //initialiserer heisen ved å kjøre til første gyldige etasje og gå i idle
    elev_set_motor_direction(DIRN_UP);
    //q_deleteQueue();
    while(elev_get_floor_sensor_signal() ==-1) {}

    elev_set_motor_direction(DIRN_STOP);
    q_deleteQueue();

        
        

    //eventmanager
   
    
    while (1) {
        //looper gjennom alle bestillingsknapper og legger i kø
        fsm_getOrders();
        fsm_stateMachine();   
        if (elev_get_obstruction_signal())
            break;
        





        
        // Stop elevator and exit program if the stop button is pressed
        /*if (elev_get_stop_signal()) {
            //fsm_setEmergencyStop();
            //elev_set_stop_lamp(0);
            elev_set_motor_direction(0);
            break;
        }*/
     
    }
	q_printQueue();
    return 0;
}
