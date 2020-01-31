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
 * Problem Description - This program answers the following question: "During a sequential read of a 
 * large file, how much data is prefetched by the file system? (Hint: time each read and plot the 
 * time per read.)"
 * 
 ****************************************************************************************************/

/* Include all required libraries that are needed to run the program. */
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h> 
#include <stdlib.h>
#include <x86intrin.h>
#include <sys/time.h>
#include <string.h>

/*******************************************************************************************************
 * 
 * Function: main - Program will start executing from here.
 * Parameters: 
 *      - None. 
 * Preconditions: 
 *      - A 18GiB file named "Random_1" must be created and populated with random data in the "/var/tmp/" 
 *        folder. 
 *      - We assume that the cache has NOT been flushed before this program is run.  
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
    const char *filename_1 = "/var/tmp/Random_1";
    char *buffer; 
    
    /* Declare and initialize a constant read size of one block (i.e. 4096 bytes).
     * This block size was found in question 1 of the assignment.     
     */
    const int readSize = 4096; 
    
    /* Declare variables that will allow us to store the number of bytes actually 
     * read and an iteration counter.
     */
    int sizeRead;
    int i;
    
    /* Declare variables that will allow us to find the time elapsed in microseconds. */
    unsigned long long rStart, rEnd;
    const unsigned long constantConversion = 3591965958;
    struct timeval gStart, gEnd;

    /* Allocate enough buffer memory to read the desired bytes. We note that
     * one char is one byte in length. */
    buffer = malloc(readSize * sizeof(char));
    
    /* Open the file in read only mode.*/
    fileDescriptor = open(filename_1, O_RDONLY);

    /* Check that the file was able to be opened. */
    if ( fileDescriptor == -1 ) {
        
        printf("ERROR: Could not open file or file does not exist. Please refer to code comments and README.txt file. Exiting.\n");   
        free(buffer);
        exit(1);

    } // end if.
    
    /* Use a for loop that will read 4096 bytes a set number of times. */
    for( i = 0; i < 100; i++ ) {
        
        /* Print the current run for readability purposes. */
        printf("RunNum%d,", i);
        
        /* The following line was used to check the validity of the cycle counter clock. */
        // gettimeofday(&gStart, NULL);
        
        /* Start the cycle counter clock. */
        rStart = _rdtsc();
       
        /* Read the appropriate number of bytes from file. */
        sizeRead = read(fileDescriptor, buffer, readSize);
       
        /* End the cycle counter clock. */
        rEnd = _rdtsc();
        
        /* The following line was used to check the validity of the cycle counter clock. */
        // gettimeofday(&gEnd, NULL);
        
        /* Check that bytes are actually being read. */
        if( sizeRead != readSize ) {
            
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
