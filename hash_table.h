#ifndef HASH_H
#define HASH_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>


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


/**
 * @brief   Array of pointers to 'people' structs
 *          *Use pointers because of space!
 *              -You don't need space for the full table unless the table is filled
 *          *
 */

void init_hash_table(struct person_t **hash_table){
    for (int i = 0; i < TABLESIZE; i++){
        *(hash_table + i) = NULL;
    }
}


/**
 * @brief Prints the table. If it encounters a linked list, prints this list accordingly
 * 
 */
void print_hash_table(struct person_t* *table){
    printf("\n\t{\n");

    for(int i = 0; i < TABLESIZE; i++){
        //if the element is NULL
        if(!(*(table + i))){
            printf("\t%d\t---\n", i);
        }

        else if(table[i] == DELETED_NODE){
            printf("\t%d\t----<deleted>\n", i);
        }


        else{

            struct person_t* person = table[i];
            printf("\t%d\t", i);
            while(person){
                printf(!(person->next) ? "%s, %d\n" : "%s, %d -> ", person->name, person->age);
                //printf("\t%d\t%s ,%d ->",i, person->name, person->age);//printf("\t%d\t%s\t%d\n",i, (*(*(table + i))).name, table[i]->age);//
                person = person->next;
            }

        }
    }
    
    printf("\t}\n");

}


//returns a ghetto hashing value for an inputted char
unsigned int hash(char *name){
    int length = strnlen(name, MAX_NAME);
    unsigned int hash_value = 0;

    for (int i = 0; i < length; i++){
        hash_value += *(name + i);
        hash_value = (hash_value *  name[i]) % TABLESIZE;
    }
    return hash_value;
    
}


//Inserts using the Open Addressing method---specifically linear probing
void insert_person_hash_table(struct person_t *person ,struct person_t* *table){

    if(!(person)){
        return;
    }

    int index = hash(person->name);

    for(int i = 0; i < TABLESIZE; i++){
        //make sure to keep 'try_index' in bounds with modulo 
        int try_index = (index + i) % TABLESIZE;
        //if the index/hash is not already taken in the hash_table, insert the person here
        //This will allow a repeated hash index to still insert... potentially
        if(table[try_index] == NULL || table[try_index] == DELETED_NODE){
            table[try_index] = person;
            return;
        }
    }

    return;

}



void insert_dynamically(struct person_t* *hash_table){

    //initially declared 'tmp' to help with the "eating of the space char" in person_name
    //decided to just sscanf person_name back into itself.
    //char *tmp = (char *) calloc(MAX_NAME, sizeof(char));

    int person_age;
    //create space for the user name
    char *person_name = (char *) calloc(MAX_NAME, sizeof(char));

    printf("Type the person's name, please: ");
    fgets(person_name, MAX_NAME, stdin);
    sscanf(person_name, "%s", person_name);

    printf("Enter the person's age\n");
    scanf("%d", &person_age);
    //fgets(person_age, 16, stdin);

    struct person_t* new_person = (struct person_t*) malloc(sizeof(struct person_t));

    //(*new_person).name = person_name;
    strcpy((*new_person).name, person_name);
    (*new_person).age = person_age;
    new_person->next = NULL;


    insert_person_hash_table(new_person, hash_table);

}



/**
 * @brief Goal is to find a head pointer at table[index] and assign the DELETED_NODE address here. Not sure how this works for linked list members yet
 */
void delete_person_hash_table(struct person_t *person, struct person_t* *table){
   
    if(!person){
        printf("That is not a valid person_t struct\n");
        return;
    }
    //compute the hash index
    int index = hash(person->name);

    //search the table for the index associated with the name and NULLify
    for(int i = 0; i < TABLESIZE; i++){

        int try_index = (index + i) % TABLESIZE;
        
        if(table[try_index] == DELETED_NODE){
            continue;
        }
        if (table[try_index] == NULL){
            return;
        }


    /**
     * @brief probably need to make 2 different IFS
     * 
     *          One for when the encountered element is a linked list... (Test IF(table[index]->next) to determine that .next!=NULL)
     *          Another for when IF(!table[index]->next) so a simple deletion can take place without list traversing
     * 
     */
        if(table[try_index]->name == person->name){
            //printf("%s\n%s\n", BUG, table[try_index]->name);
            //Probably need to add a list traversal deletion loop. Might need free() but I don't know how that works for run-time nodes

            struct person_t* leader = table[index]->next;
            while(leader){
                free(table[index]);
                table[index] = leader;
                leader = leader->next;
            }
            //free(table[index]);

            table[try_index] = DELETED_NODE;    //DELETED_NODE is the sentinel value
            //table[try_index]->next = NULL;
            return;
        }
    }
    
    //printf("Person not found in the array\n");
    return;
}



//find a person in the table by their name
struct person_t* find_person(char* name, struct person_t* *table){
    
    //Starting index in the hash table where the person could be located
    int index = hash(name);

    //search every person_node starting with the index provided by the hash.... go forward until a NULL pointer is met
    for (int i = 0; i < TABLESIZE; i++){
        //modulo keeps the value of the try_index within bounds of the hash table's possible indeces
        int try_index = (index + i) % TABLESIZE;
        
        //Finding the deleted node address doesn't indicate anything. Continue the search
        if(table[try_index] == DELETED_NODE){
            continue;
        }

        //if the encountered Node is NULL, that means this node was never inserted into the table, so stop searching
        if(table[try_index] == NULL){
            return NULL;//return NULL;
        }
        
        //I'm not sure that the first condition is required in the IF statement
        if(table[try_index] && strncmp(table[try_index]->name, name, MAX_NAME) == 0){
        //if(table[try_index]->name == name){
            return table[try_index];
        }

    }
    //printf("%s was not found in the table\n", name);
    return NULL;
}


/**
 * @brief   To insert a head node with the External Chaining method... Simply make the 'person' argument the head of linked list.
 */
void insert_head_external_chaining_method(struct person_t* person, struct person_t* *table){

    //if a NULL node is fed to the first argument
    if(!person){
        return;//return false;
    }

    int index = hash(person->name);

    //create a temporary node to copy the contents of the 'person' argument, so as to not modify 'person'
    struct person_t* head = NULL;

    //dedicate space and check if the memory was allocated
    head = malloc(sizeof(struct person_t));
    
    if(!head){
        printf("Error: malloc failed\n");
        exit(1);
    }
    
    //copy 'person' to head
    *head = *person;
    
    if(table[index] == DELETED_NODE){
        table[index] = head;
        return;
    }
    //since we're inserting a head pointer, point head's next to the table[index]
    head->next = table[index];

    //table[index] now is assigned to the head of the linked list, head
    table[index] = head;

    //printf("%p = &person\n%p = &tmp\n%p = &table[index]\n", &person, &tmp, &table[index]);
    //printf("%d = tmp->age\t%s = tmp->name\n", tmp->age, tmp->name);
    return;


}

void insert_tail_external_chaining(struct person_t* person, struct person_t* *table){
    
    
    int index = hash(person->name);

    struct person_t *tmp, *tail = NULL;

    tail = malloc(sizeof(struct person_t));

    if(!tail){
        printf("Allocation for the tail node failed\n");
        exit(1);
    }
    
    *tail = *person;    //for insertion 
    
    tmp = table[index];  //for traversal
    

    if(table[index] == DELETED_NODE){
        //printf("\n*******%s tried to insert at the deleted node*******\n",person->name);
        table[index] = tail;
        return;
    }

    while(tmp->next){
        tmp = tmp->next;
    }

    tmp->next = tail;  
    //tail->next = NULL;  
    
    return;
}




#endif