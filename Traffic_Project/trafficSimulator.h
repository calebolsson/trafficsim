#ifndef _trafficSimulator_h
#define _trafficSimulator_h
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#include "graph.h"
#include "queue.h"
#include "priorityQueue.h"
#include "event.h"
#include "road.h"

typedef struct TrafficData
{
    /* graph representing the road network */
    Graph* roadGraph;

    /* array of the roads of the graph in the sequence they were added to the graph */
    RoadData** arrayOfRoads;
    int numRoads;

    /* priority queue of events where the priority represents the time the event will occur */
    PriorityQueue* eventQueue;

    /* track the number of cars still in the simulator */
    int numCars;
}  TrafficData;

void printNames( );

TrafficData* createTrafficData( char* name );
void trafficSimulator( TrafficData* pTrafficData );
void freeTrafficData( TrafficData* pTrafficData );

#endif
