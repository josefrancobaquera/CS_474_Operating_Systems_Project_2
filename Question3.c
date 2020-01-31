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
 * Problem Description - This program answers the following question: "How big is the file cache? 
 * (Hint: Repeated reads to a group of blocks that fit in cache will be very fast; repeated reads 
 * to a group of blocks that don’t fit in cache will be slow.)"
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
 *      - We assume that the cache has been flushed multiple times before this program 
 *        is run. 
 *      - The cache size of the system being measured is assumed to be AT MOST 16GiB. 
 * Postconditions: 
 *      - Average time (in microseconds) that it takes to read 256MiB of data a set number of times. The first time 
 *        the second inner for loop is run, a total of 256MiB will be read. The second time the second inner
 *        for loop is run, a total of 512 MiB will be read, and so forth. 
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
    const char *filename_1 = "/var/tmp/Random_1";
    
    /* The program will read 256MiB = 268400000 bytes at a time. */
    const unsigned int readSize_1 = 268400000;
   
    /* Declare variables that will allow us to store the number of bytes actually 
     * read and iteration counters.
     */
    int sizeRead;
    int i, k;
    
    /* Declare and initialize a variable that will allow us to find the average time 
     * it took to read 256MiB a certain number of times. */
    double average = 0;
    
    /* Declare variables that will allow us to find the time elapsed in microseconds. */
    unsigned long long rStart, rEnd;
    const unsigned long constantConversion = 3591965958;
    struct timeval gStart, gEnd;
    
    /* Allocate enough buffer memory to read the desired bytes. We note that
     * one char is one byte in length. */
    buffer = malloc(readSize_1 * sizeof(char));
   
    /* For loop that will go until 16GiB are read. Since we are 
     * reading 256MiB at a time, and 16GiB = 16384 MiB, then 
     * 16384/256 = 64. */ 
    for ( i = 0; i < 64; i++ ) {
        
        /* Print the current run for readability purposes. */
        printf("RunNum%d,", i);
        
        /* Open the 16GiB file in read only mode. Check that it opened successfully. */
        fileDescriptor = open(filename_1, O_RDONLY);

        if ( fileDescriptor == -1 ) {
            
            printf("ERROR: Could not open file or file does not exist. Please refer to code comments and README.txt file. Exiting.\n");   
            free(buffer);
            exit(1);
            
        } // end if.
        
        /* For loop that will read 256MiB of data a certain number of times. This for loop
         * will, in essence, store the bytes read in cache. 
         */
        for( k = 0; k < i+1; k++ ) {
        
            /* Read 256MiB of data. */
            sizeRead = read(fileDescriptor, buffer, readSize_1);
            
            /* Check that the correct number of bytes were read. */
            if( sizeRead != readSize_1 ) {
            
                printf("ERROR: Program did not read the specified number of bytes (i.e. 256MiB). Exiting.\n");
                free(buffer);
                exit(1);
            
            } // end if.
            
        } // end for.
        
        /* Close file. Check if error occured. */
        if ( close(fileDescriptor) == -1 ) {
            
            printf("ERROR: Program did not close file correctly. Exiting.\n");
            free(buffer);
            exit(1);
            
        } // end if.
        
        /* Open the 16GiB file in read only mode again. This will simulate "seeking" to the
         * beginning of the file. Check that it opened successfully.*/
        fileDescriptor = open(filename_1, O_RDONLY);

        if ( fileDescriptor == -1 ) {
            
            printf("ERROR: Could not open file or file does not exist. Please refer to code comments and README.txt file. Exiting.\n");   
            free(buffer);
            exit(1);
            
        } // end if.
                
        /* The following line was used to check the validity of the cycle counter clock. */
        // gettimeofday(&gStart, NULL);
        
        /* For loop that will read 256MiB of data a certain number of times. This for loop
         * will, in essence, be slower if these bytes are not in cache. 
         */
        for( k = 0; k < i+1; k++ ) {
            
            /* Start the cycle counter clock. */
            rStart = _rdtsc();
            
            /* Read 256MiB of data. */
            sizeRead = read(fileDescriptor, buffer, readSize_1);
            
            /* End the cycle counter clock. */
            rEnd = _rdtsc();
        
            /* Keep track of the average time it took to read 256MiB of data. */
            average=average + ((((double) (rEnd -rStart)/constantConversion)  *1000000) / (i+1));
        
            /* Check that the correct number of bytes were read. */
            if( sizeRead != readSize_1 ) {
            
                printf("ERROR: Program did not read the specified number of bytes (i.e. 256MiB). Exiting.\n");
                free(buffer);
                exit(1);
            
            } // end if.
            
        } // end for.
        
       /* Print the average time it took to read the bytes (microseconds) and reset the average variable. */
        printf("%f\n", average);
        average = 0;
        
        /* The following line was used to check the validity of the cycle counter clock. */
        // gettimeofday(&gEnd, NULL);
        
        /* The following line was used to check the validity of the cycle counter clock. */
        // printf("%f microseconds\n", ((gEnd.tv_sec - gStart.tv_sec) * 1000000) + (gEnd.tv_usec - gStart.tv_usec) );
        
        /* Close file and check if error occured. This will allow the next time it is opened to seek back to the
         * the beginning of the file.
         */
        if ( close(fileDescriptor) == -1) {
            
            printf("ERROR: Program did not close file correctly. Exiting.\n");
            free(buffer);
            exit(1);
            
        } // end if.
        
    } // end outer for.
    
    /* Free the buffer. */
    free(buffer);
   
} // end main function
