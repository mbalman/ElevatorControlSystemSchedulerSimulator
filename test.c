/*-----------------------------------------------------------*
 *  Elevator Control Simulator + Scheduler 
 *                                                 balman@2016
 *-----------------------------------------------------------*/

#include<stdlib.h>
#include<stdio.h>
#include<errno.h>

/*-----------------------------------------------------------*/
#include "auxilary.h"
#include "elevator.h"

/*--------------------------------------------------------------*/
/* TODO: TESTING: Need to implement a seperate passenger object to simulate passengers!!!*/


/*test_getFloor is called when there is a call (pickup) served, so a dropoff_elevControl can be executed*/
floor_t test_getFloor(count_t elevid, floor_t goal, direc_t dir){
        /* decide here which floor button to press (goal) when in the elevator(elev_id), 
            we should associate this with pick up request, so there is up/down direction to select passenger */
        return NO_FLOOR;
}//test_getFloor

/*--------------------------------------------------------------*/
int main(){
    elevControl_t e;

    init_elevControl(&e, 2, 10, 3);
    pickup_elevControl(&e,0,ELEV_UP);
    pickup_elevControl(&e,2,ELEV_UP);
    pickup_elevControl(&e,3,ELEV_DOWN);
    pickup_elevControl(&e,4,ELEV_UP);
    pickup_elevControl(&e,5,ELEV_DOWN);

    printf(" c/t %d/%d, %d/%d --- %s %s\n",e.elevators[0].current, e.elevators[0].target, e.elevators[1].current, e.elevators[1].target, e.calls.up, e.calls.down); 
    step_elevControl(&e,1);

    printf(" c/t %d/%d, %d/%d --- %s %s\n",e.elevators[0].current, e.elevators[0].target, e.elevators[1].current, e.elevators[1].target, e.calls.up, e.calls.down); 
    step_elevControl(&e,1);

    printf(" c/t %d/%d, %d/%d --- %s %s\n",e.elevators[0].current, e.elevators[0].target, e.elevators[1].current, e.elevators[1].target, e.calls.up, e.calls.down); 

    step_elevControl(&e,1);
    printf(" c/t %d/%d, %d/%d --- %s %s\n",e.elevators[0].current, e.elevators[0].target, e.elevators[1].current, e.elevators[1].target, e.calls.up, e.calls.down); 

    dropoff_elevControl(&e,0,9); // I know one elevator at 5 now
    step_elevControl(&e,1);

    printf(" c/t %d/%d, %d/%d --- %s %s\n",e.elevators[0].current, e.elevators[0].target, e.elevators[1].current, e.elevators[1].target, e.calls.up, e.calls.down); 

    step_elevControl(&e,1);
    printf(" c/t %d/%d, %d/%d --- %s %s\n",e.elevators[0].current, e.elevators[0].target, e.elevators[1].current, e.elevators[1].target, e.calls.up, e.calls.down); 

    step_elevControl(&e,5);
    printf("after 5 steps\n");
    printf(" c/t %d/%d, %d/%d --- %s %s\n",e.elevators[0].current, e.elevators[0].target, e.elevators[1].current, e.elevators[1].target, e.calls.up, e.calls.down); 
    //
    step_elevControl(&e,15);
    printf("after 15 steps\n");
    printf(" c/t %d/%d, %d/%d --- %s %s\n",e.elevators[0].current, e.elevators[0].target, e.elevators[1].current, e.elevators[1].target, e.calls.up, e.calls.down); 
    term_elevControl(&e);
    
    return 0;
}//main
/*--------------------------------------------------------------*/

