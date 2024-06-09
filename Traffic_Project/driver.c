#include <stdlib.h>
#include <stdio.h>

#include "trafficSimulator.h"

void testFilename( char* filename, int enableTest );

int main( int argc, char *argv[] )
{
    int i;
    int numFiles = 5;
    /* defaultFilenames contains file names to be tested.  
	 * Set i-th int in enableTest to zero to disable the i-th test 
	 */
    char *defaultFilenames[] = {"./InputFiles/data-Trivial1.txt", 
								"./InputFiles/data-Trivial2.txt", 
								"./InputFiles/data-Simple.txt", 
								"./InputFiles/data-Merge1.txt", 
								"./InputFiles/data-Merge2.txt"
								};
    int enableTest[]         = { 	1,  //data-Trivial1.txt
									1,  //data-Trivial2.txt
									1,  //data-Simple.txt
									1,  //data-Merge1.txt
									1   //data-Merge2.txt
									};
		
	printNames( );

    if( argc==2 )  /* Usage: ./driver <filename> */
    {
        testFilename( argv[1], 1 );
    }
    else
    {
        printf("\nRunning default test files: \n\n");

        for( i=0; i<numFiles; i++ )
        {
			testFilename( defaultFilenames[i], enableTest[i] );
        }
    }
		
	printNames( );

    return 0;
}

/* testFilename
 * input: a string, an int
 * output: none
 *
 * Calls the student code on the given filename if enableTest is 1
 */
void testFilename( char* filename, int enableTest )
{
	
	if( enableTest== 1 )//Only run tests with a 1 in enableTest
    {
		printf("\n\n------------------ START OF OUTPUT FOR %s --------------------\n\n", filename);
		printf("\n\t\t\tREAD DATA FROM FILE\n\n");
		TrafficData* pTrafficData = createTrafficData( filename );
		printf("\n\n\t\t\tSIMULATE THE ROAD NETWORK\n\n");
		trafficSimulator( pTrafficData );
		printf("\n\n\t\t\tFREE THE SIMULATION DATA\n\n");
		freeTrafficData( pTrafficData );
		printf("\n------------------ END OF OUTPUT FOR %s --------------------\n\n", filename);
    }
    else
		printf("\n\n------------------ SKIPPED TESTING FOR %s --------------------\n\n", filename);  
}