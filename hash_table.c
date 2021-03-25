#include "hash_table.h"


/**
 * @brief       CREATE SOME PERSON STRUCTS.... INSERT THEM INTO AN ARRAY OF STRUCT POINTERS     
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