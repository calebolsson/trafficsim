#include "event.h"

/* Functions for creating and freeing events. */

/* createAddCarEvent
 * input: int timeStep, RoadData *pRoadData, Queue* pq
 * output: Event*
 *
 * This function creates and prints a new ADD_CAR_EVENT event.
 */
Event* createAddCarEvent( int timeStep, RoadData *pRoadData, Queue* pq )
{
    Event* e = (Event*)malloc(sizeof(Event));

    e->eventTimeStep = timeStep;
    e->eventCode = ADD_CAR_EVENT;
    e->pCarQueue = pq;
    e->pRoadData = pRoadData;
	
	printEvent(e);

    return e;
}

/* createAccidentEvent
 * input: int timeStep, RoadData *pRoadData
 * output: Event*
 *
 * This function creates and prints a new ROAD_ACCIDENT_EVENT event.
 */
Event* createAccidentEvent( int timeStep, RoadData *pRoadData )
{
    Event* e = (Event*)malloc(sizeof(Event));

    e->eventTimeStep = timeStep;
    e->eventCode = ROAD_ACCIDENT_EVENT;
    e->pCarQueue = NULL; //No queue is needed
    e->pRoadData = pRoadData;
	
	printEvent(e);

    return e;
}

/* createRoadClosedEvent
 * input: int timeStep, RoadData *pRoadData
 * output: Event*
 *
 * This function creates and prints a new ROAD_RESOLVED_EVENT event.
 */
Event* createResolvedEvent( int timeStep, RoadData *pRoadData )
{
    Event* e = (Event*)malloc(sizeof(Event));

    e->eventTimeStep = timeStep;
    e->eventCode = ROAD_RESOLVED_EVENT;
    e->pCarQueue = NULL; //No queue is needed
    e->pRoadData = pRoadData;
	
	printEvent(e);

    return e;
}

/* freeEvent
 * input: Event*
 * output: void
 *
 * This function frees the given event (including the queue if it is a ADD_CAR_EVENT).
 */
void freeEvent( Event* e )
{
    if( e->eventCode == ADD_CAR_EVENT )
        freeQueue( e->pCarQueue );
    free( e );
}

/* printEvent
 * input: a pointer to a Event
 * output: none
 *
 * prints the event.
 */
void printEvent( Event* e )
{
	char* eventString = "";
	
	if( e==NULL ){
		printf("Event is NULL\n" );
		return;
	}
	
	if( e->eventCode == ADD_CAR_EVENT )
        eventString = "ADD_CAR_EVENT";
	else if( e->eventCode == ROAD_ACCIDENT_EVENT )
        eventString = "ROAD_ACCIDENT_EVENT";
	else if( e->eventCode == ROAD_RESOLVED_EVENT )
        eventString = "ROAD_RESOLVED_EVENT";
	
	printf("%-19s for time step %3d on road from %3d to %3d.\n", eventString, e->eventTimeStep, e->pRoadData->from, e->pRoadData->to );
	if( e->eventCode == ADD_CAR_EVENT ){
		printf("Destinations of added cars: ");
		printQueue( e->pCarQueue );
	}
}

/* printQueue
 * input: a pointer to a Queue
 * output: none
 *
 * prints the destinations of the cars in the queue.
 */
void printQueue( Queue *pq )
{
	LLNode* temp;
	
	if( pq==NULL ){
		printf( "The queue pointer is NULL\n" );
		return;
	}
	
	temp = pq->qFront;
	if( temp==NULL ){
		printf( "The queue is empty\n");
	}
	
    while( temp!=NULL && temp->pNext!=NULL ){
		printf( "%d, ", temp->qt->destination );
		temp = temp->pNext;
	}
	printf( "%d\n", temp->qt->destination );
}