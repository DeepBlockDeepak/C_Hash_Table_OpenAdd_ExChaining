/**
 * @file    hash_table.c
 * @author  Jordan Medina (jordan.medina@student.nmt.edu)
 * @brief   Does hash table work, using Open Addressing and External Chaining
 * @date    2021-03-26
 * @details Necessary files: hash_table_functions.c and hash_table.h to compile
 */

#include "hash_table.h"


/**
 * @brief   Creates the un-initialized hash table, whose elements are struct pointers.
            Then initializes the table to contain all NULL pointers.

            Allows the user to choose a hash table function to perform. 
            Entire script quits when the user is done.
 
 CREATE SOME PERSON STRUCTS.... INSERT THEM INTO AN ARRAY OF STRUCT POINTERS

 */
int main(){

    struct person_t* hash_table[TABLESIZE];

    init_hash_table(hash_table);


    int user_choice = 0;

    while (user_choice != QUIT){

        user_choice = print_menu();

        if (user_choice == QUIT){
            printf("Bye!\n");
            break;
        }
        
        control_to_function_calls(user_choice, hash_table);

    }

    return 0;
}