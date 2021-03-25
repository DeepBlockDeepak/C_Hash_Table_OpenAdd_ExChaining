#ifndef HASH_H
#define HASH_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>


enum day {
    OPEN_ADDRESS_INSERT = 1, EXTERN_HEAD, EXTERN_TAIL, FIND, DELETE, PRINT, QUIT
};
#define TABLESIZE 10
#define MAX_NAME 256
#define BUG "\n*******BUG********\n"

//When a Node is deleted, give it this sentinel value, rather than NULL, so that search times are reduced
//Used for the Open Addressing- Linear Search method
#define DELETED_NODE (struct person_t*)(0xFFFFFFFFFFFFFFFFUL)

struct person_t{
    char name[MAX_NAME];
    int age;
    
    //added this next pointer once all the initial functions were created
    //using a linked list to perform external chaining method
    struct person_t *next;

};


int print_menu();

void control_to_function_calls(int user_choice, struct person_t* hash_table[]);

void init_hash_table(struct person_t **hash_table);

void print_hash_table(struct person_t* *table);

unsigned int hash(char *name);

void insert_person_hash_table(struct person_t *person ,struct person_t* *table);

void insert_dynamically(struct person_t* *hash_table, int user_choice_for_type_of_insertion);

void delete_person_hash_table(struct person_t *person, struct person_t* *table);

struct person_t* find_person(struct person_t* *table);

void insert_head_external_chaining_method(struct person_t* person, struct person_t* *table);

void insert_tail_external_chaining(struct person_t* person, struct person_t* *table);

#endif