#include "road.h"

/* createRoadData
 * input: int length, int from, int to, int greenStartTime, int greenEndTime, int lightCycleLength
 * output: a pointer to a RoadData (this is malloc-ed so must be freed eventually!)
 *
 * Creates a new empty RoadData and returns a pointer to it.
 */
RoadData* createRoadData( int length, int from, int to, int greenStartTime, int greenEndTime, int lightCycleLength )
{
    int i;
	
    RoadData* pr = (RoadData *)malloc( sizeof(RoadData) );
    Queue* pq = createQueue();
    Car** ppc = (Car **)malloc( sizeof(Car*)*length );
	if( pr==NULL || pq==NULL || ppc==NULL )
    {
        printf("ERROR - malloc of RoadData failed\n");
        exit(-1);
    }	
	
    pr->length = length;
    pr->waitingCars = pq;
    pr->roadContents = ppc;
    pr->from = from;
    pr->to = to;
    pr->greenStartTime = greenStartTime;
    pr->greenEndTime = greenEndTime;
    pr->lightCycleLength = lightCycleLength;
	pr->numAccidents = 0;

    for( i=0; i<length; i++ ){
        pr->roadContents[i] = NULL;
    }
	
	printRoadData( pr );				//Prints the data for the road that was just created

    return pr;
}

/* printRoadData
 * input: int length, int from, int to, int greenStartTime, int greenEndTime, int lightCycleLength
 * output: prints the given road
 *
 * Call this function to print out the road data you read in 
 */
void printRoadData( RoadData* pr )
{
	int i;
	char state='R';
	
	printf("Created road from %d to %d with length %d (green=%d; red=%d; reset=%d).\n", pr->from, pr->to, pr->length, pr->greenStartTime, pr->greenEndTime, pr->lightCycleLength );
	printf("Cycle number: ");
	for( i=0; i<25; i++ ){
		printf("%2d ", i );
	}
	printf("...\nLight state : ");	
	if(PRINT_COLOR) 
		printf("\033[0;31m");//Color = RED
	for( i=0; i<25; i++ ){	/* print example light cycle: */
		if( i%pr->lightCycleLength==pr->greenStartTime ){
			if(PRINT_COLOR) 
				printf("\033[0;32m");//Color = GREEN
			state = 'G';
		}
		if( i%pr->lightCycleLength==pr->greenEndTime ){
			if(PRINT_COLOR) 
				printf("\033[0;31m");//Color = RED
			state = 'R';
		}
		printf("%2c ", state );
	}
	if(PRINT_COLOR) 
		printf("\033[0m");//Reset color
	printf("...\n\n");
}

/* createRoadData
 * input: a pointer to a RoadData to be freed
 * output: none
 *
 * Frees malloc-ed data in RoadData
 */
void freeRoadData( RoadData* pr )
{
    int i;

    for( i=0; i<pr->length; i++ ){
        if( pr->roadContents[i] != NULL )
            free( pr->roadContents[i] );
    }
    freeQueue( pr->waitingCars );
    free( pr->roadContents );
    free( pr );
}


/* printRoadContents
 * input: a pointer to a RoadData
 * output: N/A
 *
 * Print the destinations of the cars on the road
 */
void printRoadContents( RoadData* pr )
{
    int i;
	
	if( !ENABLE_PRINT_ROAD )
		return; //The printing of the roads is disabled

    printf("Cars on the road from %d to %d: \n", pr->from, pr->to);
    for( i=0; i<pr->length; i++ ){
        if( pr->roadContents[i]!= NULL )
            printf("%d ", pr->roadContents[i]->destination );
        else
            printf("- ");
    }
	if( pr->currentLightState == GREEN_LIGHT && pr->numAccidents==0 && PRINT_COLOR)
		printf("\033[0;32m");//Color = GREEN
	else if( PRINT_COLOR )
		printf("\033[0;31m");//Color = RED
	
    if( pr->currentLightState == GREEN_LIGHT )
        printf("(GREEN light - " );
    else
        printf("(RED light   - " );
	
	if( pr->numAccidents>0 )
		printf("The number of unresolved accidents = %d)\n", pr->numAccidents );
	else if ( pr->numAccidents==0 )
		printf("No unresolved accidents)\n" );
	else //negative number of accidents means something has gone wrong
		printf("ERROR there are %d unresolved accidents)\n", pr->numAccidents );
	
	if(PRINT_COLOR) 
		printf("\033[0m");//Reset color
}

