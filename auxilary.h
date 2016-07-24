/*-----------------------------------------------------------*
 *  Elevator Control Simulator + Scheduler 
 *                                                 balman@2016
 *-----------------------------------------------------------*/
/*                      AUXILARY HEADER                      */ 
/*-----------------------------------------------------------*/
#ifndef ELEV_AUX
#define ELEV_AUX

#include<stdlib.h>
#include<errno.h>

/*-----------------------------------------------------------*/
#define NO_FLOOR         -1
#define ELEV_UP           1
#define ELEV_DOWN        -1
#define ELEV_IDLE         0
#define FALSE             0
#define TRUE              1
#define CALL_NONE         '0'
#define CALL_SET          'N'
#define CALL_SCHED        'S'
typedef int             floor_t;        // floor ids,min value is 0, <0 means none
typedef short           direc_t;              
typedef unsigned short  count_t;                        
typedef char            call_t;         // call_t 

/*-------------------------------------------------------------*/
/*-------        stopList_t      ------------------------------*/
/*-------------------------------------------------------------*/

// bookeeping for floors in which elevator will stop
typedef struct stopList_st{     
        floor_t * list;         // list of floors to stop (pickUp/dropOff)
        count_t  num;           // number of items in the list
        count_t capacity;       // max number of items / capacity
// No need to keep capacity for each stopList, they are all same for every elevator (they could be different), but for simplicity I pass capacity info to stopList */
    }stopList_t;
/* stopList could be a TreeSet in JAVA. In the interest of time, I implemented as a simple array */

/*-------------------------------------------------------------*/
/*-------     callRequest         -----------------------------*/
/*-------------------------------------------------------------*/
typedef struct callRequest_st{
    call_t * up;
    call_t * down;
    }callRequest_t;
/* callRequest is a simple key,value hashTable */

/*-------------------------------------------------------------*/

void        init_callRequest(callRequest_t *, count_t numFloor);
void        term_callRequest(callRequest_t *);

/*-----------------------------------------------------------*/

stopList_t * create_stopList(count_t capacity);
void         term_stopList(stopList_t *);
int          search_stopList(stopList_t *, floor_t);
int          add_stopList(stopList_t *, floor_t);
count_t      num_stopList(stopList_t *);
count_t      cap_stopList(stopList_t *);
int          del_stopList(stopList_t *, floor_t);
floor_t      target_stopList(stopList_t *, direc_t,floor_t curTarget);

direc_t      opposite_direc(direc_t);
/*-----------------------------------------------------------*/

void * mymalloc(size_t );   // do error checking here!

/*-----------------------------------------------------------*/

#endif

