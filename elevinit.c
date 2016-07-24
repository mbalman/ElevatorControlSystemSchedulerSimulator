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

void  dropoff_elevControl(elevControl_t * ctr, count_t id, floor_t floor){
        if (!ctr) perror("update_elevControl: NULL"), exit(3);
        if ( floor >= ctr->numFloor || id >= ctr->numElev) return; 
        if (ctr->elevators[id].current == floor) return; //already at the same floor
        add_stopList(ctr->elevators[id].stops,floor); /* if elevator is full, ignore the request */
}//dropoff_elevControl

/*--------------------------------------------------------------*/
void pickup_elevControl(elevControl_t * ctr, floor_t floor, direc_t dir){
        if (!ctr) perror("pickup_elevatorControl: NULL"), exit(3);
        if (floor >= ctr->numFloor) return ;
        if (dir==ELEV_IDLE) return; //ignore - it should be up or down
        if (floor==0 && dir==ELEV_DOWN) return;
        if ((floor==(ctr->numFloor-1) )&& (dir==ELEV_UP)) return;
        setCall_elevControl(ctr,floor,dir); //will be ignored if there is already a request
}//pickup_elevatorControl

/*--------------------------------------------------------------*/

direc_t dir_elevator(elevator_t *elev){
//      if (!elev) perror("dir_elevator: NULL"), exit(3);
        if (elev->target > elev->current) 
                return ELEV_UP;
        if (elev->target < elev->current)
                return ELEV_DOWN;
        return ELEV_IDLE;  //target == current
}//dir_elevator

/*--------------------------------------------------------------*/

direc_t dir_elevControl(elevControl_t *ctr, count_t elevid){
        if (!ctr) perror("dir_elevControl: NULL"), exit(3);
        if (elevid >= ctr->numElev) return ELEV_IDLE;
        return dir_elevator(&(ctr->elevators[elevid]));
}//dir_elevControl

floor_t cur_elevControl(elevControl_t *ctr, count_t elevid){
        if (!ctr) perror("cur_elevControl: NULL"), exit(3);
        if (elevid >= ctr->numElev) return 0;
        return ctr->elevators[elevid].current;
}//cur_elevControl

/*--------------------------------------------------------------*/
/* if elevator is idle, then choose up or down:
        if it was previous going down, choose down - if it was previous going up, choose up
        if it is floor 0, then choose up - if it is at top floor, chose down */
direc_t indir_elevControl(elevControl_t *ctr, count_t elevid){
    elevator_t * elev;
    direc_t dir;
        //if (!ctr) perror("direction_elevControl: NULL"), exit(3);
        //if (elevid >= ctr->numElev) return ELEV_IDLE;

        elev=&(ctr->elevators[elevid]);
        dir=dir_elevator(elev);
        if (dir!=ELEV_IDLE) return dir; //target != current

        if (elev->current == (ctr->numFloor -1)) elev->indir=ELEV_DOWN;
        if (elev->current == 0) return elev->indir=ELEV_UP;
        return elev->indir;
}//indir_elevControl

/*--------------------------------------------------------------*/

void move_elevator(elevator_t * elev){
    direc_t dir;
            dir=dir_elevator(elev);
            if (dir==ELEV_IDLE) return;
            if (dir==ELEV_UP){
                            elev->current++;
                            elev->indir=ELEV_UP;
                            return;
            }//if
            elev->current--;
            elev->indir=ELEV_DOWN;
}//move_elevator

/*--------------------------------------------------------------*/

inline void initVals_elevator(elevator_t *elev){
        elev->current=0;          //all elevators at floor 0
        elev->target=0;           //all elevators at floor 0
        elev->indir=ELEV_UP;    
}//initVals

void init_elevator(elevator_t * elev, count_t capacity){
        if (!elev) perror("init_elevator: NULL"), exit(3);
        initVals_elevator(elev);
        elev->stops=create_stopList(capacity);
}//init_elevator

void term_elevator(elevator_t * elev){
        if (!elev) perror("term_elevator: NULL"), exit(3);
        if (elev->stops){
                term_stopList(elev->stops);
                free(elev->stops);
                elev->stops=NULL; //double check
            }//if       
        initVals_elevator(elev);
}//term_elevator

/*--------------------------------------------------------------*/

void init_elevControl(elevControl_t * ctr, count_t eNum, count_t fNum, count_t capacity){
    count_t e;
        if (!ctr) perror("init_elevControl: NULL"), exit(3);
        if (eNum <=0 || fNum <=0 || capacity <=0) perror("init_elevControl: invalid arguments"), exit(4);
        /* create space for elevator objects*/
        ctr->elevators=(elevator_t *) mymalloc(sizeof(elevator_t)*eNum);
        for (e=0; e<eNum; e++) /* initialize elevator objects*/
             init_elevator(&(ctr->elevators[e]),capacity);
        /* create space for callRequests. Each one represents up/down buttons for floors*/ 
        init_callRequest(&(ctr->calls),fNum);
        ctr->numElev=eNum;
        ctr->numFloor=fNum;
}//init_elevControl 

void term_elevControl(elevControl_t * ctr){
    count_t i;
        if (!ctr) perror("term_elevControl: NULL"), exit(3);
        for (i=0; i< (ctr->numElev);i++)
            term_elevator(&(ctr->elevators[i]));
        term_callRequest(&(ctr->calls));
        ctr->numElev=0;
        ctr->numFloor=0;
}//term_elevControl

/*--------------------------------------------------------------*/
