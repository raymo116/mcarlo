/*
* A simple program to illustrate multithreading by using a
* monte-carlo simultion.
*
* Based on a program by Gagne, Galvin, and Silberschatz
*
* @name: mcarlo.c
* @author: Matt Raymond
* @email: raymo116@mail.chapman.edu
* @id: 2270559
* @date: 03/15/2020
* @version: 1.0
*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// The number of points inside the circle
int numInside = 0;
// The total number of points
int numTotal = 0;

/*
* Performs a monte-carlo simulation to approximate pi
* Input: Takes in a param void* param as the range to cycle over
* Output: Returns a type (void *) of the number counted
*/
void *monteCarloSim(void* param);

/*
* Generates a double precision random number
* Input: None
* Output: Returns a random double between 0 and 1
*/
double random_double();

/*
* This program will use multithreading to create nultiple threads to run a
* monte-carlo simulation in parallel. The usage is `./a.out <integer value>`
*/
int main(int argc, char *argv[])
{
    // thread identifiers
    pthread_t tid1;
    pthread_t tid2;

    // Checks to make sure that the correct number of parameters were added
    if (argc != 2) {
        fprintf(stderr,"usage: ./a.out <integer value>\n");
        return -1;
    }

    // Checks to see if the input is positive
    if (atoi(argv[1]) < 0) {
        fprintf(stderr,"Argument %d must be non-negative\n",atoi(argv[1]));
        return -1;
    }

    // Saves the total number so we don't have to keep casting it
    numTotal = atoi(argv[1]);

    // Creates two parameters, one for each thread
    int *arg1 = malloc(sizeof(int));
    int *arg2 = malloc(sizeof(int));

    // Splits the argument up for the threads
    *arg1 = (int)floor(numTotal/2);
    *arg2 = (int)ceil(numTotal/2);

    // Creates threads
    pthread_create(&tid1,NULL,monteCarloSim,arg1);
    pthread_create(&tid2,NULL,monteCarloSim,arg2);

    // Gets values back from threads
    pthread_join(tid1,NULL);
    pthread_join(tid2,NULL);

    // numInside = (float)((int)intReturn1 + (int)intReturn2);

    // Prints the output from the computation
    printf("Estimate = %f\n",(4.0*numInside/numTotal));

    // Frees up the memory used
    free(arg1);
    free(arg2);
}

/*
* Performs a monte-carlo simulation to approximate pi
* Input: Takes in a param void* param as the range to cycle over
* Output: Returns a type (void *) of the number counted
*/
void *monteCarloSim(void* param)
{
    // Casts the original value
    int value = *((int *) param);
    int internalSum = 0;

    // Performs the monte-carlo simulation
    for (int i = 0; i < value; i++) {
        // Makes the number between 0 and 2, and then -1 and 1
        float x = random_double() * 2.0 - 1.0;
        float y = random_double() * 2.0 - 1.0;

        // Finds the euclidean distance between the origin and the given point
        if (sqrt(x*x + y*y) < 1.0) {
            ++internalSum;
        }
    }

    // Appends to the global variable
    numInside += (float)internalSum;

    pthread_exit(0);
}

/*
* Generates a double precision random number
* Input: None
* Output: Returns a random double between 0 and 1
*/
double random_double() {
    return random() / ((double)RAND_MAX + 1);
}
