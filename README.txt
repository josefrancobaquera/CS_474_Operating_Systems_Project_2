********* HEADER *********

OSProjectTwo 

CS474: Operating Systems
Project #2 (File System Measurements)
December 10, 2019
 
Team Member 1: 
Liliana Aguirre Esparza
 
Team Member 2: 
Jose Franco Baquera

Problem Description: In this project, we will pick any Unix-based platform to study and will perform some simple 
experiments that will bring out various properties of the file system under test. The properties that this project
focuses on include block size, how much data is pre-fetched, cache size, and allocation method.

********* ASSUMPTIONS *********

- We ran our programs on the CS Linux machines only. As such, we assume that our code will be 
  compiled and run on Linux machines only.  
- Our program will find the main memory cache size of a system only up to 16 GiB. That is, Question3.c
  will fail for a system that has a cache size larger than 16 GiB.
- There should be a folder named "/var/tmp/" in the filesystem and at least 60 GiB of free disk space.
- The source code Question4.c will only work for systems that are extent-based.
- We recommend using the CS lab machines.
- PLEASE USE THE hopper HOST to run our code!!!! This will give you similar results to the ones provided in our 
  report. Please ssh to hopper using the following command:

	ssh CSUserName@hopper.cs.nmsu.edu

********* WARNING *********

- We had to run our programs multiple times to see patterns. That is, running our programs only once will
  not be suffice. 
- You can make the program run in one CPU and thus reduce the effects of multi-programming by using the 
  following command taskset --cpu-list (e.g. taskset --cpu-list 1 ./Question1 512)
  We note, however, that running our programs this way provided no noticeable differences than when running them 
  normally. 
  
********* BEFORE RUNNING ANY PROGRAM, MAKE SURE TO COMPLETE THE FOLLOWING LIST OF STEPS!!! *********

-- Steps 1 --
Change the current working directory (using the command line) to the folder where these files are located.

-- Steps 2 --
Run the following commands to compile our code:
make Q1
make Q2
make Q3
make Q4

-- Steps 3 --
Run the following commands to create the necessary files:
make createLargeWrite
make createRandom_1
make createRandom_2

It is worth noting that these commands might take a few seconds or minutes to complete.

********* RUNNING Question1.c CODE *********

-- Steps 1 --
Decide the number of bytes you want to read at a time. Let us call it this number "A".

-- Steps 2 --
Run the program in the following manner: ./Question1 A
For example, if I want to read 512 bytes at a time, I would use the following command: ./Question1 512

NOTE: This should be run multiple times to see a consistent pattern in the results!!
NOTE: Do not clear cache!!!!!!!!!!!

********* RUNNING Question2.c CODE *********

-- Steps 1 --
Run the program in the following manner: ./Question2

NOTE: This program will not provide meaningful results for cache sizes larger than 16 GiB.
NOTE: This should be run multiple times to see a consistent pattern in the results!!
NOTE: Do not clear cache!!!!!!!!!!!!!!

********* RUNNING Question3.c CODE *********

-- Steps 1 --
Flush the cache by running the following command MULTIPLE times:
make flushCache

-- Steps 2 --
Run the program in the following manner: ./Question3

NOTE: This should be run multiple times (with clearing the cache) to see a consistent pattern in the results!!!!!
NOTE: Clear the cache every time program is run!!!

********* RUNNING Question4.c CODE *********

-- Steps 1 --
Run the program in the following manner: ./Question4

NOTE: This should be run multiple times to see a consistent pattern in the results!!
NOTE: This program will provide meaningful results for extent-based systems only. 

