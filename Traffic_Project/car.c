#include "car.h"

/* Functions for creating and freeing cars. */

/* createCar
 * input: int stepAdded, int origin, int next, int destination
 * output: Car*
 *
 * Creates and returns a Car* with given data.  The Car* is malloced so be sure to eventually call freeCar to free it.
 */
Car* createCar( int stepAdded, int origin, int next, int destination )
{
    Car* c = (Car*)malloc(sizeof(Car));

    c->stepAdded = stepAdded;
    c->origin = origin;
    c->next = next;
    c->destination = destination;

    c->moved = false;

    return c;
}

void freeCar( Car* c )
{
    free( c );
}
