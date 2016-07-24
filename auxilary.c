/*-----------------------------------------------------------*
 *  Elevator Control Simulator + Scheduler 
 *                                                 balman@2016
 *-----------------------------------------------------------*/
#include<stdlib.h>
#include<stdio.h>
#include<errno.h>
/*-----------------------------------------------------------*/
#include "auxilary.h"

/*--------------------------------------------------------------*/

void init_callRequest(callRequest_t * call, count_t numFloor){
    count_t i;
        if (!call) return; //TODO: do better error checkking
        call->up=(call_t*)mymalloc(sizeof(call_t)*(numFloor+1)); 
        call->down=(call_t*)mymalloc(sizeof(call_t)*(numFloor+1)); 
        for (i=0;i<numFloor;i++){
            call->up[i]=CALL_NONE;
            call->down[i]=CALL_NONE;
        }//for
        call->up[i]=call->down[i]='\0'; //debug
}//init_callRequest

void  term_callRequest(callRequest_t * call){
      if (!call) return;
      if (call->up) free(call->up);
      if (call->down) free(call->down); 
      call->up=NULL;
      call->down=NULL;  
}//term_callRequest

/*--------------------------------------------------------------*/

stopList_t * create_stopList(count_t capacity){
    stopList_t * stops;
    count_t i;
        stops=(stopList_t *)mymalloc(sizeof(stopList_t));
        stops->num=0;
        stops->capacity=capacity;
        stops->list=(floor_t*)mymalloc(sizeof(floor_t)*capacity);
        for (i=0;i<capacity;i++)
                stops->list[i]=NO_FLOOR;
        //for
        return stops;
}//create_stopList

void  term_stopList(stopList_t * stops){
        if (!stops) return;
        if (stops->list) free(stops->list);
        stops->list=NULL;
        stops->capacity=0;
        stops->num=0;
}//term_stopList

int search_stopList(stopList_t * stops, floor_t f){
    count_t i;   
        if (!stops) return FALSE; 
        for (i=0;i<stops->capacity;i++)
            if (stops->list[i]==f) return TRUE;
        return FALSE;
}//search_stopList

inline count_t num_stopList(stopList_t * stops){
    //if (!stop) perror("stops is NULL"),exit(4); //Assert
        return stops->num;
}//num_stopList

inline count_t cap_stopList(stopList_t * stops){
    //if (!stop) perror("stops is NULL"),exit(4); //Assert
        return stops->capacity;
}//cap_stopList

int del_stopList(stopList_t * stops, floor_t f){
    count_t i;
        if (!stops) return FALSE;
        if (stops->num <=0) return FALSE;
        for (i=0;i<stops->capacity;i++)
            if (stops->list[i]==f) {
                    stops->list[i]=NO_FLOOR;
                    stops->num--;
                    return TRUE;
             }//if
        return FALSE;
}//del_stopList

int  add_stopList(stopList_t * stops, floor_t f){
    count_t i;
        if (!stops) return FALSE;
        if (stops->num >= stops->capacity) return FALSE;
        if (search_stopList(stops,f)) //already exists
                    return TRUE;
        for (i=0;i<stops->capacity;i++)
            if (stops->list[i]==NO_FLOOR){
                    stops->list[i]=f;
                    stops->num++;
                    return TRUE;
            }//if
        return FALSE;
}//add_stopList

floor_t  max_stopList(stopList_t * stops){
    count_t i;
    floor_t f=NO_FLOOR; // no floor is the smallest 
        if (!stops) return f;
        for (i=0;i<stops->capacity;i++)
            if (stops->list[i]>f) f=stops->list[i];
        return f;
}//max_stopList

floor_t min_stopList(stopList_t * stops){
    count_t i;
    floor_t f=NO_FLOOR;
        if (!stops || (stops->num <=0)) return NO_FLOOR;
        for (i=0;i<stops->capacity;i++){
            if (stops->list[i]==NO_FLOOR) continue;
            if (f==NO_FLOOR) f=stops->list[i];
            else if (f < (stops->list[i])) f=stops->list[i];
        }//for
        return f;
}//min_stopList

/* TARGET: get max of min in one direction only*/
floor_t  target_stopList(stopList_t * stops, direc_t dir,floor_t curTarget){
    floor_t tmp;
        if (dir==ELEV_UP){
                tmp=max_stopList(stops);
                if (tmp!=NO_FLOOR && (tmp>curTarget)) return tmp;
        }//ELEV_UP
        if (dir==ELEV_DOWN){
                tmp=min_stopList(stops);
                if (tmp!=NO_FLOOR && ( tmp < curTarget)) return tmp;
        }//ELEV_DOWN
        return curTarget;
}//target_stopList

inline direc_t opposite_direc(direc_t dir){
        if (dir==ELEV_UP) return ELEV_DOWN;
        if (dir==ELEV_DOWN) return ELEV_UP;
        return dir;
}//direction_change


/*--------------------------------------------------------------*/
void * mymalloc(size_t size){
    void * tmp;
        if(!(tmp=malloc(size))) perror("malloc failed"),exit(2);
        return tmp;
}//mymalloc
/*--------------------------------------------------------------*/
