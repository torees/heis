//
//  ElevatorStateMachine.h
//  Heisprosjekt
typedef enum{
    state_idle,
    state_run,
    state_doorOpen,
    state_emergencyStopValidFloor,
    state_emergencyStopInvalidFloor,
    state_invalidFloorIdle
} EState;



static int currentFloor;
static int priorityDirection;
static int targetFloor;
int fsm_emergencyStop;

void fsm_setEmergencyStop();
int fsm_getNextInQueue();
void fsm_getOrders();
void fsm_setPriorityDirection(int direction);
//void fsm_setState(EState state);
void fsm_stateMachine(); 

/* statemachine
whatToDo (, &g_state)
switch currentstate
case state_idle
	if queuenotempty 
		currrentstate = moving
	break
case moving
	if floorReached
		currentState = state_doorOpen
*/

