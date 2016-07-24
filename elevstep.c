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
#ifdef ELEV_TEST        // if ELEV_TEST is defined call this function 
    floor_t test_getFloor(count_t,floor_t,direc_t);
#endif
inline void t_delCall_elevControl(elevControl_t * ctr, floor_t current, direc_t dir, count_t elevid){
            delCall_elevControl(ctr,current,dir);
#ifdef ELEV_TEST
    /* Passenger can get in and press any button inside at anytime.  To test the simulation, here play a drop request */
            dropoff_elevControl(ctr,elevid,test_getFloor(elevid,current,dir));
#endif
}//t_delCall_elevControl

/*--------------------------------------------------------------*/
/* TODO: optimize by keeping a nextStop variable in elevator, so we do not need to check stopList unnecessarily
         update nextStop when stopList is updated (when stop added or deleted)*/
void procStop_elevControl(elevControl_t * ctr){
    count_t ec;
    direc_t indir;
    elevator_t *elev;
        for (ec=0; ec < (ctr->numElev); ec++){
            elev=&(ctr->elevators[ec]); // to make the code more readable

/* either I have a stop request or my target is set to this floor for a pickup */
            if ( del_stopList(elev->stops,elev->current) ||    \
             (  (elev->current==elev->target)                  \
                    && isCallAny_elevControl(ctr,elev->current) \
                    && ( num_stopList(elev->stops) < cap_stopList(elev->stops)) ) ){
/* if there are two or more elevators to pickup at the same floor for the same direction, I do not do load balancing based on capacity, just based on index. could put all stopping elevators in a hashmap and then pick one  */             

                   indir=indir_elevControl(ctr,ec);
                   /* I just drop someone, or my target was set to here and I have capacity*/
                   if(isCall_elevControl(ctr,elev->current,indir)) //pick a call request on your direction
                            t_delCall_elevControl(ctr,elev->current,indir,ec);

                   /* I have no other stop below/above (my direction)  and there was no call request on my direction*/ 
                   else if( (elev->current==elev->target)    \
                                && isCall_elevControl(ctr,elev->current, opposite_direc(indir)) ) 
                                t_delCall_elevControl(ctr,elev->current,opposite_direc(indir),ec);
            }//if stop the elevator             
        }//for     
}//procStop_elevControl

/*--------------------------------------------------------------*/
/* TODO: get rid of this function, optimize to update target:
            - when elevator stops at a floor 
            - when a stop request is added */
void procTarget_elevControl(elevControl_t * ctr){
    count_t ec;
    direc_t indir;
    elevator_t * elev;
        for (ec=0; ec < (ctr->numElev); ec++){
            indir=indir_elevControl(ctr,ec);
            elev=&(ctr->elevators[ec]);
            
            elev->target=target_stopList(elev->stops,indir,elev->target); //get target in your direction
            if (elev->current==elev->target) //reached its top or bottom, try opposite direction
                elev->target=target_stopList(elev->stops,opposite_direc(indir),elev->target);
        }//for
}//procTarget_elevControl

void move_elevControl(elevControl_t * ctr){
    count_t ec;
        for (ec=0; ec < (ctr->numElev); ec++)
            move_elevator(&(ctr->elevators[ec]));
}//move_elevControl

/*--------------------------------------------------------------*/
void step_elevControl(elevControl_t * ctr, count_t steps){
    count_t s;
        if (!ctr) perror("step_elevControl: NULL"), exit(3);
        for (s=0; s<steps; s++){
            sched_elevControl(ctr);     //schedule 
            procStop_elevControl(ctr);  //process stops and pickups
            procTarget_elevControl(ctr);//update targets if necessary
            move_elevControl(ctr);
        }//for
}//step_elevControl
/*--------------------------------------------------------------*/

