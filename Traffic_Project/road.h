#ifndef _road_h
#define _road_h
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "car.h"
#include "queue.h" 


#define PRINT_COLOR false  /* Print color coding for printRoadData (may not work outside of Linux environment) */
#define ENABLE_PRINT_ROAD true  /* Set this to FALSE to disable printing the road contents on each step */

typedef enum trafficLightType{ GREEN_LIGHT, RED_LIGHT } trafficLightType;

typedef struct RoadData
{
    /* The length of this road (i.e. # of cars that can fit on this road) */
    int length;

    /* The information used to record/update whether the light at the end of this road is green or red */
    trafficLightType currentLightState;
    int greenStartTime;
    int greenEndTime;
    int lightCycleLength;

    /* The intersections this road starts from and moves to */
    int from;
    int to;
	
    /* A counter of how many accidents are currently unresolved on this road.  
	 * Only if this counter is 0 can a Car can make a move related to this road. 
	 */
    int numAccidents;

    /* Each road has a `Car**` array to track cars currently driving on them.  The type of this is`Car**` 
     * (array of car pointers) and locations on the road can be set to `NULL` if no car is present.
     */
	 Car** roadContents;

    /* Each road has a `queue` of Car* variables waiting to enter the simulated road system (see `queue.h` for 
     * the queues functions which have already been implemented for you).  Cars will be put into this  
	 * queue only when an add car event occurs for this road.
     */
    Queue* waitingCars;
}  RoadData;

RoadData* createRoadData( int length, int from, int to, int greenStartTime, int greenEndTime, int lightCycleLength );
void printRoadData( RoadData* pr );
void freeRoadData( RoadData* pr );
void printRoadContents( RoadData* pr );

#endif

