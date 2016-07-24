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

inline int isCallAny_elevControl(elevControl_t * ctr, floor_t floor){
        if (floor >= ctr->numFloor) return FALSE; //assert TODO: do better error checking in call functions
        if (ctr->calls.up[floor]!=CALL_NONE) return TRUE;
        if (ctr->calls.down[floor]!=CALL_NONE) return TRUE;
        return FALSE;
}//isCallAny_elevControl

inline int isCall_elevControl(elevControl_t * ctr, floor_t floor, direc_t dir){
        if (dir==ELEV_UP)
            if(ctr->calls.up[floor]!=CALL_NONE) return TRUE;
        if (dir==ELEV_DOWN)
            if(ctr->calls.down[floor]!=CALL_NONE) return TRUE;
        return FALSE;
}//isCall_elevControl
        
inline int isNew_elevControl(elevControl_t * ctr, floor_t floor, direc_t dir){
        if (dir==ELEV_UP)
            if(ctr->calls.up[floor]==CALL_SET) return TRUE;
        if (dir==ELEV_DOWN)
            if(ctr->calls.down[floor]==CALL_SET) return TRUE;
        return FALSE;
}//isNew_elevControl

inline void setCall_elevControl(elevControl_t * ctr, floor_t floor, direc_t dir){
        if (dir==ELEV_UP)
            if(ctr->calls.up[floor]==CALL_NONE) //if it is CALL_SET or CALL_SCHED, ignore
                    ctr->calls.up[floor]=CALL_SET;
        if (dir==ELEV_DOWN)
            if(ctr->calls.down[floor]==CALL_NONE) 
                    ctr->calls.down[floor]=CALL_SET;
}//setCall_elevControl

inline void delCall_elevControl(elevControl_t * ctr, floor_t floor, direc_t dir){
        if (dir==ELEV_UP)
                    ctr->calls.up[floor]=CALL_NONE;
        if (dir==ELEV_DOWN)
                    ctr->calls.down[floor]=CALL_NONE;
}//delCall_elevControl

inline void setSched_elevControl(elevControl_t * ctr, floor_t floor, direc_t dir){
        if (dir==ELEV_UP)
                    ctr->calls.up[floor]=CALL_SCHED;
        if (dir==ELEV_DOWN)
                    ctr->calls.down[floor]=CALL_SCHED;
}//setSched_elevControl
