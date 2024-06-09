#include "trafficSimulator.h"

// optional TODO: Put your new function declarations here
int max(int a, int b);

/* printNames
 * input: none
 * output: none
 *
 * Prints names of the students who worked on this solution
 * REMINDER: You may only work with one other cs2124 student
 */
void printNames()
{
    /* TODO : Fill in you and your partner's names (or N/A if you worked individually) */
    printf("\nThis solution was completed by:\n");
    printf("<Matthew Bonnichsen>\n");
    printf("<N/A>\n");
}

/* createTrafficData
 * input: char* filename of file to read
 * output: TrafficData* which stores the data of this road network
 *
 * Create a new TrafficData variable and read in the traffic data from the file whose name is stored in char* filename
 */
TrafficData *createTrafficData(char *filename)
{
    /* open the file */
    FILE *pFile = fopen(filename, "r");

    if (pFile == NULL)
    {
        printf("ERROR - the file \"%s\" was not found!\n", filename);
        exit(-1);
    }

    TrafficData *data = malloc(sizeof(TrafficData));
    int intersection;
    int roads;
    int carEvents;
    int roadEvents;
    int i;
    int j;
    int k;

    /* TODO: read in all the data in pFile */
    /* HINT: use fscanf( pFile, "%d", &<your int variable here> ) to read an int from pFile */

    fscanf(pFile, "%d %d", &intersection, &roads);
    printf("Intersection: %d\n", intersection);
    /*fscanf(pFile, "%d", &roads);*/
    printf("Roads: %d\n", roads);
    data->numRoads = roads;
    printf("numRoads: %d\n", data->numRoads);
    printf("Roads: %d\n", roads);

    data->arrayOfRoads = malloc(roads * sizeof(RoadData *));
    data->roadGraph = createGraph(intersection);

    for (i = 0; i < roads; i++)
    {
        int from, to, length, greenOn, greenOff, reset;

        fscanf(pFile, "%d", &from);
        fscanf(pFile, "%d", &to);
        fscanf(pFile, "%d", &length);
        fscanf(pFile, "%d", &greenOn);
        fscanf(pFile, "%d", &greenOff);
        fscanf(pFile, "%d", &reset);

        data->arrayOfRoads[i] = createRoadData(from, to, length, greenOn, greenOff, reset);

        setEdge(data->roadGraph, from, to, length);
        setEdgeData(data->roadGraph, from, to, data->arrayOfRoads[i]);
    }

    fscanf(pFile, "%d", &carEvents);
    data->numCars = carEvents;
    Queue *carQueue = createQueue();
    data->eventQueue = createPQ();

    for (i = 0; i < carEvents; i++)
    {
        int from, to, timeStep, numCarsToAdd, destination;

        fscanf(pFile, "%d", &from);
        fscanf(pFile, "%d", &to);
        fscanf(pFile, "%d", &timeStep);
        fscanf(pFile, "%d", &numCarsToAdd);

        for (j = 0; j < numCarsToAdd; j++)
        {
            fscanf(pFile, "%d", &destination);

            for (k = 0; k < roads; k++)
            {
                if (data->arrayOfRoads[k]->from == from && data->arrayOfRoads[k]->to == to)
                {
                    Car *tempCar = createCar(timeStep, from, to, destination);
                    enqueue(carQueue, tempCar);
                    Event *tempEvent = createAddCarEvent(timeStep, data->arrayOfRoads[k], carQueue);
                    enqueueByPriority(data->eventQueue, tempEvent, timeStep);
                    break;
                }
            }
        }
    }

    fscanf(pFile, "%d", &roadEvents);

    for (i = 0; i < roadEvents; i++)
    {
        int from, to, timeStep, duration;

        fscanf(pFile, "%d", &from);
        fscanf(pFile, "%d", &to);
        fscanf(pFile, "%d", &timeStep);
        fscanf(pFile, "%d", &duration);

        for (k = 0; k < roads; k++)
        {
            if (data->arrayOfRoads[k]->from == from && data->arrayOfRoads[k]->to == to)
            {
                Event *tempAccident;
                Event *tempResolved;
                tempAccident = createAccidentEvent(timeStep, data->arrayOfRoads[k]);
                tempResolved = createResolvedEvent(timeStep + duration, data->arrayOfRoads[k]);
                enqueueByPriority(data->eventQueue, tempAccident, timeStep);
                enqueueByPriority(data->eventQueue, tempResolved, timeStep);
                break;
            }
        }
    }

    /* close file */
    fclose(pFile);

    freeQueue(carQueue);

    return data;
}

/* trafficSimulator
 * input: char* name of file to read
 * output: N/A
 *
 * Simulate the road network in the given TrafficData variable
 */
void trafficSimulator(TrafficData *pTrafficData)
{
    /*Queue distance;*/
    int timeStep;
    int j;
    int i;
    /*roadData->numAccidents = 0;*/
    const int MAX_STEPS = 1000;
    int maxTime = 0;
    int sumTime = 0;
    int numCars = 0;
    RoadData *roadData = (RoadData *)malloc(sizeof(RoadData));
    /*Queue* pq = createQueue();
    Car** ppc = (Car **)malloc( sizeof(Car*)*length );*/

    /* Loop until all events processed and either all cars reached destination */
    /* You can also assume all test data will finish in <1000 steps */
    /* This fact can be used to help you avoid infinite loops while completing the project */
    for (timeStep = 0; timeStep < MAX_STEPS; timeStep++)
    {
        /* Print the current step number */
        printf("Current Step Number: %d\n", timeStep);

        /* Update the state of every traffic light */
        for (j = 0; j < pTrafficData->numRoads; j++)
        {
            roadData = pTrafficData->arrayOfRoads[j];

            if (timeStep % roadData->lightCycleLength == 0)
                roadData->currentLightState = RED_LIGHT;

            if (roadData->greenStartTime == timeStep % roadData->lightCycleLength)
                roadData->currentLightState = GREEN_LIGHT;

            if (roadData->greenEndTime == timeStep % roadData->lightCycleLength)
                roadData->currentLightState = RED_LIGHT;
        }

        /* Loop on events associated with this time step */
        while (!isEmptyPQ(pTrafficData->eventQueue) && getFrontPriority(pTrafficData->eventQueue) == timeStep)
        {
            Event *thisEvent = getNextPQ(pTrafficData->eventQueue);
            roadData = thisEvent->pRoadData;
            /* If ADD_CAR_EVENT, use mergeQueues from queue.h to add the queue of cars to the queue of the road associated with the event */
            if (thisEvent->eventCode == ADD_CAR_EVENT)
                mergeQueues(roadData->waitingCars, thisEvent->pCarQueue);

            /* If ROAD_ACCIDENT_EVENT, add one to the numAccidents of the road associated with the event */
            if (thisEvent->eventCode == ROAD_ACCIDENT_EVENT)
            {
                roadData->numAccidents++;
                printf("Adding accident to road from %d to %d", roadData->from, roadData->to);
            }

            /* If ROAD_RESOLVED_EVENT, subtract one from the numAccidents of the road associated with the event */
            if (thisEvent->eventCode == ROAD_RESOLVED_EVENT)
                if (roadData->numAccidents > 0)
                {
                    roadData->numAccidents--;
                    printf("Removing accident to road from %d to %d", roadData->from, roadData->to);
                }

            dequeuePQ(pTrafficData->eventQueue);
        }

        /* Print the contents of each road (use the provided function printRoadContents) */
        for (j = 0; j < pTrafficData->numRoads; j++)
        {
            printRoadContents(pTrafficData->arrayOfRoads[j]);
        }

        /* For each road, try to move waiting cars onto the end of that road if possible (remember to check that numAccidents==0 for the road) */
        for (j = 0; j < pTrafficData->numRoads; j++)
        {
            roadData = pTrafficData->arrayOfRoads[j];
            if (roadData->length == 0)
                continue;
            if (roadData->numAccidents == 0 && roadData->roadContents[roadData->length - 1] == NULL)
            {
                roadData->roadContents[roadData->length - 1] = dequeue(roadData->waitingCars);
            }
        }

        /* For each road, try to move cars, which haven't already moved, forward one space on every road (remember to check that numAccidents==0 for the road) */
        for (j = 0; j < pTrafficData->numRoads; j++)
        {
            roadData = pTrafficData->arrayOfRoads[j];
            if (roadData->numAccidents == 0)
            {
                for (i = 1; i < roadData->length; i++)
                {
                    if (roadData->roadContents[i] != NULL && roadData->roadContents[i - 1] == NULL && !roadData->roadContents[i]->moved)
                    {
                        roadData->roadContents[i]->moved = true;
                        roadData->roadContents[i - 1] = roadData->roadContents[i];
                        roadData->roadContents[i] = NULL;
                    }
                }
            }
        }

        /* For each road, try to move cars, which haven't already moved, through the next intersection (remember to check that numAccidents==0 for both roads) */
        for (j = 0; j < pTrafficData->numRoads; j++)
        {
            roadData = pTrafficData->arrayOfRoads[j];

            if (roadData->numAccidents == 0 && roadData->currentLightState == GREEN_LIGHT && roadData->length != 0 && roadData->roadContents[0] != NULL && !roadData->roadContents[0]->moved)
            {
                if (roadData->to == roadData->roadContents[0]->destination)
                {
                    sumTime += timeStep - roadData->roadContents[0]->stepAdded;
                    numCars++;
                    maxTime = max(timeStep, maxTime);
                    freeCar(roadData->roadContents[0]);
                    roadData->roadContents[i] = NULL;
                    continue;
                }

                roadData->roadContents[0]->moved = true;
                graphType mewTwo; /*the new intersection or 'to' we are heading to*/
                if (getNextOnShortestPath(pTrafficData->roadGraph, roadData->to, roadData->roadContents[0]->destination, &mewTwo))
                {
                    enqueue(getEdgeData(pTrafficData->roadGraph, roadData->to, mewTwo)->waitingCars, roadData->roadContents[0]);
                    roadData->roadContents[0] = NULL;
                }
            }
        }

        for (j = 0; j < pTrafficData->numRoads; j++)
        {
            roadData = pTrafficData->arrayOfRoads[j];
            for (i = 0; i < roadData->length; i++)
                if (roadData->roadContents[i] != NULL)
                    roadData->roadContents[i]->moved = false;
        }
    }

    /* After the loop finishes print the average and max number of steps it took for a car to reach its destination */
    if (numCars == 0)
    {
        numCars = 1;
    }
    int avgTime = sumTime / numCars;
    printf("Average number of time steps to the reach their destination is %d.\n", avgTime);
    printf("Maximum number of time steps to the reach their destination is %d.\n", maxTime);

    for (j = 0; j < pTrafficData->numRoads; j++)
    {
        roadData = pTrafficData->arrayOfRoads[j];
        for (i = 0; i < roadData->length; i++)
            if (roadData->roadContents[i] != NULL)
            {
                freeCar(roadData->roadContents[i]);
                roadData->roadContents[i] = NULL;
            }
        freeQueue(roadData->waitingCars);
        freeRoadData(roadData);
    }

    freeTrafficData(pTrafficData);
}
/* freeTrafficData
 * input: TrafficData* pTrafficData
 * output: N/A
 *
 * Free the data in the TrafficData variable
 */
void freeTrafficData(TrafficData *pTrafficData)
{
    int i;

    freeGraph(pTrafficData->roadGraph);
    freePQ(pTrafficData->eventQueue);
    for (i = 0; i < pTrafficData->numRoads; i++)
        freeRoadData(pTrafficData->arrayOfRoads[i]);
    free(pTrafficData->arrayOfRoads);
    free(pTrafficData);
}

int max(int a, int b)
{
    if (a > b)
        return a;
    return b;
}