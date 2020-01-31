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
 * Problem Description - This program answers the following question: "What is the allocation method 
 * for the file system? For file systems using indirect block mapping scheme, how many direct 
 * pointers are in the inode? For systems using extent-based scheme, how big is the extent size? 
 * (Hint: think about using write() and fsync() to answer this question. Or use read(). Also, think 
 * about what’s the differences between the indirect block mapping scheme and the extent-based scheme.)"
 *
 * NOTE: This program assumes that the system being tested uses an extent-based scheme. 
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
 *      - An empty 16 GiB file named "LargeTestFile" must be created inside the "/var/tmp/" folder. 
 *      - We assume that the system being tested uses an extent-based scheme.
 * Postconditions: 
 *      - Time (in microseconds) that it takes to write 1 MiB of random data to disk.
 * Return:
 *      - No return value.
 * 
 ********************************************************************************************************/
int main ( int argc, char *argv []) {

    /* Declare a set of variables that will allow us to 
     * open the file and store the file descriptor, a location 
     * of where to store 1 MiB of random data, and a pointer string
     * of the file's path. */
    int fileDescriptor;
    const char *filename_1 = "/var/tmp/LargeTestFile";
    char *buffer; 
    
    /* Declare and initialize a constant write size of 1 MiB (i.e. 1049000 bytes).*/
    const int writeSize = 1049000; 
    
    /* Declare iterative variable used for looping. */
    int i;
    
    /* Declare variables that will allow us to find the time elapsed in microseconds. */
    unsigned long long rStart, rEnd;
    const unsigned long constantConversion = 3591965958;
    struct timeval gStart, gEnd;

    /* Allocate a buffer of size 1 MiB. This buffer will have "garbage" in it, which is fine.
     * The important thing is that we will write the 1 MiB of garbage to the temporary large 
     * test file. We note that one char is one byte in length.*/
    buffer = malloc(writeSize * sizeof(char));
    
    /* Open the file in read and write mode.*/
    fileDescriptor = open(filename_1, O_RDWR);

    /* Check that the file was able to be opened. */
    if ( fileDescriptor == -1 ) {
        
        printf("ERROR: Could not open file or file does not exist. Please refer to code comments and README.txt file. Exiting.\n");   
        free(buffer);
        exit(1);

    } // end if.
    
    /* Use a for loop that will write 1 MiB of random data at a time to the large test file.*/
    for( i = 0; i < 350; i++ ) {
        
        /* Print the current run for readability purposes. */
        printf("RunNum%d,", i);
        
	/* Write to file system cache and check that the correct number of bytes are written. */
	if( write(fileDescriptor, buffer, writeSize ) == -1 ) {
            
	    printf("ERROR: Program did not write the specified number of bytes. Exiting.\n");
            free(buffer);
            exit(1);        

	} // end if  

	/* The following line was used to check the validity of the cycle counter clock. */
        // gettimeofday(&gStart, NULL);

        /* Start the cycle counter clock. */
        rStart = _rdtsc();
       
        /* Flush all data stored in buffers to actual disk space and check that it is done
	 * successfully. */
        if( fsync(fileDescriptor ) == -1 ) {
            
            printf("ERROR: Program did not write data to disk successfully. Exiting.\n");
            free(buffer);
            exit(1);

        } // end if 

        /* End the cycle counter clock. */
        rEnd = _rdtsc();
        
        /* The following line was used to check the validity of the cycle counter clock. */
        // gettimeofday(&gEnd, NULL);
         
        /* Print the time it took to read the bytes (microseconds). */
        printf("%f\n", ((double) (rEnd -rStart)/constantConversion) *1000000);
        
        /* The following line was used to check the validity of the cycle counter clock. */
        // printf("%f microseconds\n", ((gEnd.tv_sec - gStart.tv_sec) * 1000000) + (gEnd.tv_usec - gStart.tv_usec) );

    } // end for.

    /* Free the allocated memory. */
    free(buffer);

} // end main function
