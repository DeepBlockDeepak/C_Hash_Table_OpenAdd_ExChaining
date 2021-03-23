CC = gcc
CFLAGS = -g -Wall
OBJFLAGS = -g -Wall -c
MATHLIB = -lm

targets = hash_table
clean_targets = hash_table

all : $(targets)

#creates the object file, list_functions.o, necessary for compilation
#Including the header file list.h as a safety, to tell the user that it is necessary to be there
hash_table : hash_table.c hash_table.h
	$(CC) $(CFLAGS) $@.c -o $@

#clears the object files and executables from the directory.
clean : 
	rm $(clean_targets)

