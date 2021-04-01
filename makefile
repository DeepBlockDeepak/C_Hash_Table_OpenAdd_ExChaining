CC = gcc
CFLAGS = -g -Wall
OBJFLAGS = -g -Wall -c
MATHLIB = -lm

targets =  hash_table
clean_targets = hash_table

all : $(targets)

#creates the object file, hash_table_functions.o, necessary for compilation
#Including the header file hash_table.h as a safety, to tell the user that it is necessary to be there
hash_table_functions : hash_table_functions.c hash_table.c hash_table.h 
	$(CC) $(OBJFLAGS) $@.c

hash_table : hash_table.c hash_table.h hash_table_functions.o
	$(CC) $(CFLAGS) hash_table_functions.o $@.c -o $@

#clears the object files and executables from the directory.
clean : 
	rm *.o $(clean_targets)

