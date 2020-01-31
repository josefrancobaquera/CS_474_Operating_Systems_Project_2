# ****************************************************************************************************
# * 
# * CS474: Operating Systems
# * Project #2 (File System Measurements)
# * December 10, 2019
# * 
# * Team Member 1: 
# * Liliana Aguirre Esparza
# * 
# * Team Member 2: 
# * Jose Franco Baquera
# * 
# ****************************************************************************************************

# Default make should error.
all:
	$(error "Nothing to do with all; Must be more specific. Please refer to the makefile for more precise commands")

# Deleting files (sometimes) also clears the system's cache.
deleteRandom_1:
	rm /var/tmp/Random_1
deleteRandom_2:
	rm /var/tmp/Random_2
deleteLargeWrite:
	rm /var/tmp/LargeTestFile

# Creating the three necessary files to run the programs.
createLargeWrite:
	fallocate -l 16GB /var/tmp/LargeTestFile
createRandom_1:
	base64 /dev/urandom | head -c 18G > /var/tmp/Random_1
createRandom_2:
	base64 /dev/urandom | head -c 25G > /var/tmp/Random_2

# Flush cache by catting large file.
flushCache: 
ifneq (,$(wildcard /var/tmp/Random_2))
	cat /var/tmp/Random_2 > /dev/null
else
	base64 /dev/urandom | head -c 25G > /var/tmp/Random_2
	cat /var/tmp/Random_2 > /dev/null
endif

# Compiling the four programs needed to solve the four questions.
Q1:
	gcc Question1.c -o Question1
Q2:
	gcc Question2.c -o Question2
Q3:
	gcc Question3.c -o Question3
Q4:
	gcc Question4.c -o Question4
