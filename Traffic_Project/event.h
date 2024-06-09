#ifndef _event_h
#define _event_h
#include <stdbool.h>

#include "queue.h"
#include "road.h"

typedef enum eventType { ADD_CAR_EVENT, ROAD_ACCIDENT_EVENT, ROAD_RESOLVED_EVENT } eventType;

typedef struct Event
{
    eventType eventCode;    /* Type of event */
    int eventTimeStep;      /* Time step associated with this event */
	RoadData *pRoadData;    /* RoadData associated with the event (use getEdgeData from graph.c to retrieve this) */

    /* ADD_CAR_EVENT */
    Queue *pCarQueue;       /* Queue of Car* variables associated with the event */
	
    /* ROAD_ACCIDENT_EVENT */
    //No additional variables needed
	
    /* ROAD_RESOLVED_EVENT */
    //No additional variables needed

}  Event;

Event* createAddCarEvent( int timeStep, RoadData *pRoadData, Queue* pq );
Event* createAccidentEvent( int timeStep, RoadData *pRoadData );
Event* createResolvedEvent( int timeStep, RoadData *pRoadData );
void freeEvent( Event* e );

void printEvent( Event* e );
void printQueue( Queue *pq );

#endif
