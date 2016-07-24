/*-----------------------------------------------------------*
 *  Elevator Control Simulator + Scheduler 
 *                                                 balman@2016
 *-----------------------------------------------------------*/
/*              ELEVATOR HEADER                              */
/*-----------------------------------------------------------*/

#include "auxilary.h"

#ifndef ELEV_CONTROL
#define ELEV_CONTROL

/*-----------------------------------------------------------*/
/*-------     elevator     ----------------------------------*/
/*-----------------------------------------------------------*/
typedef struct elevator_st{
       floor_t      current;     // current floor
       floor_t      target;      // target floor for the elevator
       direc_t      indir;       // intended direction - to break ties when current==target 
       stopList_t   *stops;      // list of floors this elevator will stop 
    }elevator_t;
/*-----------------------------------------------------------*/
/*-------     elevControl   ---------------------------------*/
/*-----------------------------------------------------------*/
typedef struct elevControl_st{
        count_t         numElev;        // number of elevators in the system
        count_t         numFloor;       // number of floors in the system - min floor_id is 0
        elevator_t      * elevators;    // list of elevators. index is elevator id
        callRequest_t   calls;          // represents the up/down buttons in each floor
    }elevControl_t;

/*-----------------------------------------------------------*/
/*---         EXTERNAL INTERFACES                ------------*/
/*-----------------------------------------------------------*/
void    step_elevControl(elevControl_t *, count_t);
void    pickup_elevControl(elevControl_t *, floor_t, direc_t);     //press button outside
void    dropoff_elevControl(elevControl_t *, count_t, floor_t);     //press button inside 
direc_t dir_elevControl(elevControl_t *, count_t);
floor_t cur_elevControl(elevControl_t *, count_t);

//EXTERNAL constructor, desctructor 
void    init_elevControl(elevControl_t *, count_t elevNum, count_t floorNum, count_t capacity);
void    term_elevControl(elevControl_t *);

/*-----------------------------------------------------------*/
/*---         INTERNAL                           ------------*/

void    init_elevator(elevator_t *, count_t capacity);
void    term_elevator(elevator_t *);

direc_t dir_elevator(elevator_t *);                     //direction of an elevator
direc_t indir_elevControl(elevControl_t *, count_t);    //intended direction

void    procStop_elevControl(elevControl_t *); // TODO: optimize to do all together/embedded into one function
void    procTarget_elevControl(elevControl_t *);
void    move_elevator(elevator_t *);
void    move_elevControl(elevControl_t *);

void    sched_elevControl(elevControl_t *);   // TODO:
int     find_elevator(elevControl_t *, floor_t, direc_t, floor_t *);
int     find_elevator_second(elevControl_t *, floor_t, direc_t, floor_t *);
void    sched_firstRound(elevControl_t *, floor_t, direc_t);
void    sched_secondRound(elevControl_t *, floor_t, direc_t);

/*-----------------------------------------------------------*/

int     isCallAny_elevControl(elevControl_t *, floor_t );
int     isCall_elevControl(elevControl_t *, floor_t, direc_t);
int     isNew_elevControl(elevControl_t *, floor_t, direc_t);
void    setCall_elevControl(elevControl_t *, floor_t, direc_t);
void    setSched_elevControl(elevControl_t *, floor_t, direc_t);
void    delCall_elevControl(elevControl_t *, floor_t, direc_t);


/*--------------------------------------------------------------*/
#endif
