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

/* find the closest elevator, that is not full, going same direction up (down), 
            and it is below (above) me or at the same floor*/
int find_elevator(elevControl_t *ctr, floor_t floor, direc_t calldir, floor_t * elevid_ptr){
    int found=FALSE;        
    direc_t indir;
    elevator_t * elev;
    count_t capdiff, distance, tmpcap, tmpdistance, elevid, i;
        for ( i=0; i<ctr->numElev; i++){
                elev=&(ctr->elevators[i]); // using elev to make code more readable
                if (calldir==ELEV_UP) {
                                    if (elev->current > floor) continue; //current <= floor
                                    tmpdistance=floor - elev->current;
                                    }//if
                if (calldir==ELEV_DOWN) {
                                    if (elev->current < floor) continue; // current >=floor
                                    tmpdistance= elev->current - floor;
                                    }//if
                tmpcap=cap_stopList(elev->stops)-num_stopList(elev->stops);
                if (tmpcap <=0 ) continue; //this elevator has no space left

                indir=indir_elevControl(ctr,i);
                if (calldir != indir) continue; // skip in this time step cycle 

/* if elevator is idle (target==current) but intended dir is down, but there is an up call at current floor: this step will not put it into stopList (if it was down call, it will); 
    why because there may be other call reqeust in this cycle going down coming from below floors,
    but procStop will pick this up call since there was no down call and target==current*/ 
                if (!found) {   // if that is the first one found
                         elevid=i;
                         distance=tmpdistance;
                         capdiff=tmpcap;
                         found=TRUE;
                }//found
                else if (tmpdistance < distance){ //select closest
                         elevid=i;
                         distance=tmpdistance;
                         capdiff=tmpcap;
                }else if (tmpdistance == distance && tmpcap > capdiff){ //select closest with more space
                         elevid=i;
                         distance=tmpdistance;
                         capdiff=tmpcap;
                }//if distance == tmpdistance
        }//for
        if (found) (*elevid_ptr)=elevid;
        return found;
}//find_elevator

/*--------------------------------------------------------------*/

/* change target of an elevator so it can came towards this call (mobilize the closest elevator):
        if it is idle (current==target) and has empty stopList,
        or it coming towards you (going opposite to your dir otherwise first pass will catch) 
             and its target is not passing you.*/ 
int find_elevator_second(elevControl_t * ctr, floor_t floor, direc_t calldir, floor_t *elevid_ptr){
    int found=FALSE;
    direc_t indir;
    elevator_t * elev;
    count_t capdiff, distance, tmpcap, tmpdistance, elevid, i;
        for ( i=0; i<ctr->numElev; i++){
                elev=&(ctr->elevators[i]); // using elev to make code more readable
                
                tmpcap=cap_stopList(elev->stops)-num_stopList(elev->stops);
                if (tmpcap <=0 ) continue; //this elevator has no space left

                if ((elev->target==elev->current) && (num_stopList(elev->stops)==0)){
                            if (floor > elev->current) 
                                        tmpdistance=floor - elev->current;
                            else  tmpdistance= elev->current - floor;
                }//idle with no stops
                else{
                    indir=indir_elevControl(ctr,i); // not indir
    
                    /*if not selected in the first phase, either above the floor for up, or below for down */
                    if (calldir ==indir) continue; 
                    if (calldir==ELEV_UP){//indir is down
                                     if (elev->current <= floor) continue; // it is below me and going down, let it go more down and then we will look again in the next time stepping cycle
                                     if (elev->target < floor) continue; //if its target is less than me, it will pass me and will go down
                                     tmpdistance=elev->current - floor;
                                     }//if
                    if (calldir==ELEV_DOWN){
                                     if (elev->current >= floor) continue; 
                                     if (elev->target > floor) continue; // it will pass me and will go up
                                     tmpdistance=floor - elev->current;
                                    }//if
                }//else

                if (!found) {   // if that is the first one found
                         elevid=i;
                         distance=tmpdistance;
                         capdiff=tmpcap;
                         found=TRUE;
                }//found
                else if (tmpdistance < distance){ //select closest
                         elevid=i;
                         distance=tmpdistance;
                         capdiff=tmpcap;
                }else if (tmpdistance == distance && tmpcap > capdiff){ //select closest with more space
                         elevid=i;
                         distance=tmpdistance;
                         capdiff=tmpcap;
                }//if distance == tmpdistance

        }//for
        if (found) (*elevid_ptr)=elevid;
        return found;
}//find_elevator_second

/*--------------------------------------------------------------*/

void sched_firstRound(elevControl_t * ctr, floor_t floor, direc_t dir){
    floor_t elevid;
        if (!isNew_elevControl(ctr,floor,dir)) return;
        if (find_elevator(ctr,floor,dir,&elevid)){  //elevid type is unsigned :(
                add_stopList(ctr->elevators[elevid].stops,floor);
                setSched_elevControl(ctr,floor,dir);
        }//if no such elevator, try to mobilize one to pick it up in the second round
}//sched_firstRound

void sched_secondRound(elevControl_t * ctr, floor_t floor, direc_t dir){
    floor_t elevid;
        if (!isNew_elevControl(ctr,floor,dir)) return;
        if (find_elevator_second(ctr,floor,dir,&elevid))  //elevid type is unsigned :(
/*!!*/          /* TODO: double check */
                    ctr->elevators[elevid].target=floor;
        // Defer scheduling to next time stepping cycle
}//sched_secondRound

/*--------------------------------------------------------------*/

void sched_elevControl(elevControl_t * ctr){
    count_t floor;
        for ( floor=0; floor < ctr->numFloor ; floor++)   //first round
/* three two up calls from 15th 10th 5th floor, if have one elevators at 3 with indir up and with capacity 2, 5 and 10 will be scheduled first, bec after 10th, it can go to 15th */
            sched_firstRound(ctr,floor,ELEV_UP);
        for( floor=(ctr->numFloor-1); ; floor-- ){
            sched_firstRound(ctr,floor,ELEV_DOWN);
            if (floor==0) break;
        }//for

        for( floor=(ctr->numFloor-1); ; floor-- ){ //second round
            sched_secondRound(ctr,floor,ELEV_UP);
            if (floor==0) break;
        }//for
        for ( floor=0; floor < ctr->numFloor ; floor++)   
            sched_secondRound(ctr,floor,ELEV_DOWN);
/* floor order is helpful but scheduling will not break if order is changed:
        eg: there are 20 floors, and 2 elevators, and all elevators idle at floor 0. if there are calls from 10th, and 5th floors to down; first, one elevator will go to 10th, and then next will go to 5th.
    - Second round will make elevators go up to reach them (by changing target). One elevator is going up to 10th floor, to pick up a down request, someone may get in and request a stop to go above 10th floor, in such a case this elevator should pass the down call at 10th floor (since it is for down, and elevator is going up)*/ 
}//sched_elevControl

