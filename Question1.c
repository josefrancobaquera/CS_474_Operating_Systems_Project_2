/****************************************************************************************************
 * 
 * CS474: Operating Systems I
 * Project #2 (File System Measurements)
 * December 10, 2019
 * 
 * Team Member 1: 
 * Liliana Aguirre Esparza
 * 
 * Team Member 2: 
 * Jose Franco Baquera 
 *
 * Problem Description - This program answers the following question: "How big is the block size used 
 * by the file system to read data? (Hint: use reads of varying sizes and plot the time it takes to do 
 * such reads. Also, be wary of prefetching effects that often kick in during sequential reads.)"
 * 
 ****************************************************************************************************/

/* Include all required libraries that are needed to run the program. */
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h> 
#include <stdlib.h>
#include <x86intrin.h>
#include <sys/time.h>

/*******************************************************************************************************
 * 
 * Function: main - Program will start executing from here.
 * Parameters: 
 *      - A read size of bytes that will be read from the "Random_1" file. 
 * Preconditions: 
 *      - A 18GiB file named "Random_1" must be created and populated with random data in the "/var/tmp/" 
 *        folder. 
 *      - We assume that the cache has not been flushed before this program is run. 
 *      - A reasonable readSizeUser of bytes. That is, the parameter inputted by the user must be a non-negative, 
 *        integer value. 
 * Postconditions: 
 *      - Time (in microseconds) that it takes to read a certain number of bytes for a set number of times.
 * Return:
 *      - No return value.
 * 
 ********************************************************************************************************/
int main ( int argc, char *argv []) {

    /* Declare a set of variables that will allow us to 
     * open the file and store the file descriptor, a location 
     * of where to store the read data, and a pointer string
     * of the file's path. */
    int fileDescriptor;
    char *buffer; 
    const char *fileName = "/var/tmp/Random_1";
    
    /* Declare variables that will allow us to store user input, 
     * the number of bytes actually read, and an iteration counter.*/
    int readSizeUser;
    int sizeRead;
    int i;
    
    /* Declare variables that will allow us to find the time elapsed in microseconds. */
    unsigned long long rStart, rEnd;
    const unsigned long constantConversion = 3591965958;
    struct timeval gStart, gEnd;

    /* Check for the correctness of user input. */
    if( argc <= 1 ) {
        
        printf("ERROR: No arguments passed. Must enter the number of bytes that are desired to be read for each iteration. Exiting.\n");      
        exit(1);
        
    } // end if.
    
    else if ( argc > 2 ) {
        
        printf("WARNING: Entered more than one argument. Will only use the first argument and ignore all other ones.\n");
        
    } // end else if.
    
    if ( atoi(argv[1]) <= 0 ) {
        
        printf("ERROR: Argument passed %d must be at least 1. Exiting.\n", atoi(argv[1]));      
        exit(1);
        
    } // end else if.
    
    /* Assign the argument to a variable and allocate enough buffer memory to read the desired bytes. We note that
     * one char is one byte in length. */
    readSizeUser = atoi(argv[1]);
    buffer = malloc(readSizeUser * sizeof(char));
    
    /* Open the file in read only mode. */ 
    fileDescriptor = open(fileName, O_RDONLY);
    
    /* Check that the file was able to be opened. */
    if ( fileDescriptor == -1 ) {
        
        printf("ERROR: Could not open file or file does not exist. Please refer to code comments and README.txt file. Exiting.\n");   
        free(buffer);
        exit(1);

    } // end if.
    
    /* Use a for loop that will read the desired number of bytes a set number of times. */
    for( i = 0; i < 100; i++ ) {
        
        /* Print the current run for readability purposes. */
        printf("RunNum%d,", i);
        
        /* The following line was used to check the validity of the cycle counter clock. */
        // gettimeofday(&gStart, NULL);
        
        /* Start the cycle counter clock. */
        rStart = _rdtsc();
       
        /* Read the appropriate number of bytes from file. */ 
        sizeRead = read(fileDescriptor, buffer, readSizeUser);
        
        /* End the cycle counter clock. */
        rEnd = _rdtsc();
        
        /* The following line was used to check the validity of the cycle counter clock. */
        // gettimeofday(&gEnd, NULL);
        
        /* Check that bytes are actually being read. */
        if( sizeRead != readSizeUser ) {
            
            printf("ERROR: Program did not read the specified number of bytes. Exiting.\n");
            free(buffer);
            exit(1);
            
        } // end if.
               
        /* Print the time it took to read the bytes (microseconds). */
        printf("%f\n", ((double) (rEnd -rStart)/constantConversion) *1000000);
        
        /* The following line was used to check the validity of the cycle counter clock. */
        // printf("%f microseconds\n", ((gEnd.tv_sec - gStart.tv_sec) * 1000000) + (gEnd.tv_usec - gStart.tv_usec) );

    } // end for.

    /* Free the allocated memory. */
    free(buffer);

} // end main function
