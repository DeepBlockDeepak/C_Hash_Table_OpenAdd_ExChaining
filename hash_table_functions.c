#include "hash_table.h"


/**
 * @brief   Prints the user_interface. Allows user to make selections for hash table functions
 * 
 * @return int      The user's choice
 */
int print_menu(){

    //buf needed to stuff away user input
    //buf material sent to the user_choice
    char buf[MAX_NAME];
    int user_choice;
    
    printf(
            "\n\tWould you like to enter a person, using:\n"
            "\t\t1) Open Addressing/Linear Probing?\n"
            "\t\t2) External Chaining- as the HEAD node?\n"
            "\t\t3) External Chaining- as the TAIL node?\n"
            "\n\tOR:\n"
            "\t\t4) Find a person?\n"
            "\t\t5) Delete a person?\n"
            "\t\t6) PRINT the Table?\n"
            "\t\t7) QUIT?\n\n"
    );
    
    fgets(buf, MAX_NAME, stdin);
    sscanf(buf, "%d", &user_choice);

    return user_choice;

}


/**
 * @brief   Takes the user's menu choice, and determines which function call needs to be performed
 * 
 * @param user_choice   Return from the print_menu() selection 
 * @param hash_table    A pointer to the hash table
 */
void control_to_function_calls(int user_choice, struct person_t* hash_table[]){

    //this struct required to accept the return of the find_person() call within the FIND and DELETE choices 
    struct person_t *person_of_interest = NULL;
    
    switch (user_choice){
    //user quits the program
    case QUIT:
        break;
    
    case PRINT:
        print_hash_table(hash_table);
        break;

    //in each case, an insertion will occur by malloc'ing a new struct 
    case OPEN_ADDRESS_INSERT:
    case EXTERN_HEAD:
    case EXTERN_TAIL:
        insert_dynamically(hash_table, user_choice);
        break;

    //checks if a name is already in the array
    case FIND:

        person_of_interest = find_person(hash_table);

        person_of_interest  ?   printf("\n\t:) %s is in the hash table**\n", (*person_of_interest).name)
                            :   printf("\n\t**That person isn't in the table.**\n");

        break;

    case DELETE:

        delete_person_hash_table(find_person(hash_table) ,hash_table);
        break;

    default:
        break;


    }


}


/**
 * @brief   Array of pointers to 'people' structs
 *          *Use pointers because of space!
 *              -You don't need space for the full table unless the table is filled
 *          
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
        
        //if the element is NULL, i.e. no person has ever entered this slot
        if(!(*(table + i))){
            printf("\t%d\t---\n", i);
        }

        //slot is neither NULL nor does it contain a person. Formerly contained a person, since deleted
        else if(table[i] == DELETED_NODE){
            printf("\t%d\t----<deleted>\n", i);
        }


        //handles any amount of nodes at the slot in the table
        else{
            
            //set a temporary pointer, person, to the slot in the table
            struct person_t* person = table[i];
            printf("\t%d\t", i);

            while(person){
                printf(
                    !(person->next) ? "%s, %d\n"        //if the current node is the very last
                                    : "%s, %d -> ",     //every other node
                                    person->name, person->age
                );

                //increment the node pointer
                person = person->next;
            }

        }
    }
    
    printf("\t}\n");

}


/**
 * @brief   returns a ghetto hashing value for an inputted char
 * 
 * @param name  the string literal of the person's name member variable
 * @return unsigned int     the value of the hash, normalized to fit inside range(0, len(hash_table) - 1)
 */
unsigned int hash(char *name){

    int length = strnlen(name, MAX_NAME);
    unsigned int hash_value = 0;

    //iterate over each char, and mix up the hash_value with squaring method.... uses modulo to keep in-bounds
    for (int i = 0; i < length; i++){
        hash_value += *(name + i);
        hash_value = (hash_value *  name[i]) % TABLESIZE;
    }
    return hash_value;
    
}


/**
 * @brief Inserts using the Open Addressing method---specifically linear probing
 * 
 * @param person    the struct node to insert into the array
 * @param table     the array of struct pointers --- the hash table
 */
void insert_open_add_method(struct person_t *person ,struct person_t* *table){

    //check here for a junk person
    if(!(person)){
        return;
    }

    int index = hash(person->name);

    //open addressing checks each available spot 
    for(int i = 0; i < TABLESIZE; i++){
        //make sure to keep 'try_index' in bounds with modulo 
        int try_index = (index + i) % TABLESIZE;
        //if the index/hash is not already taken in the hash_table, insert the person here
        if(table[try_index] == NULL || table[try_index] == DELETED_NODE){
            table[try_index] = person;
            return;
        }
    }

    //if the hash_table is fully occupied, then the person was not added
    printf("There was no more room in the table\n");
    free(person);
    return;

}



void insert_dynamically(struct person_t* *hash_table, int user_choice_for_type_of_insertion){

    //initially declared 'tmp' to help with the "eating of the space char" in person_name
    //decided to just sscanf person_name back into itself.
    //          char *tmp = (char *) calloc(MAX_NAME, sizeof(char));


    /**************         Creation of the new node, and assignment of user input to the struct variables         **************/
    
    //create space for the user name
    char *person_name = (char *) calloc(MAX_NAME, sizeof(char));
    int person_age;

    printf("Type the person's name, please: ");
    fgets(person_name, MAX_NAME, stdin);
    sscanf(person_name, "%s", person_name);

    printf("Enter the person's age: ");
    scanf("%d", &person_age);

    /**
     * @bug     SOLVED: Was having an issue with the main menu being printed twice after inserting a person.
     *          Solved by inserting the getchar() instance here. It's not cool.
     */
    while ((getchar()) != '\n');

    //creating the new struct here, and the user input will be assigned to its members
    struct person_t* new_person = (struct person_t*) malloc(sizeof(struct person_t));

    //(*new_person).name = person_name //@bug This is a char*(?) bug.... used strcpy instead
    strcpy((*new_person).name, person_name);
    (*new_person).age = person_age;
    new_person->next = NULL;

    /**************         Node has been created and is ready to be passed to the relevant function call         **************/

    //Switch depending on the method of the user's choice
    switch (user_choice_for_type_of_insertion){
    case OPEN_ADDRESS_INSERT:
        insert_open_add_method(new_person, hash_table);
        break;

    case EXTERN_HEAD:
        insert_head_external_chaining_method(new_person, hash_table);
        break;
    
    case EXTERN_TAIL:
        insert_tail_external_chaining(new_person, hash_table);
        break;

    default:
        break;

    }


}



/**
 * @brief Goal is to find a head pointer at table[index] and assign the DELETED_NODE address here. Not sure how this works for linked list members yet
 */
void delete_person_hash_table(struct person_t *person, struct person_t* *table){
   
   //Guard against a trash person
    if(!person){
        printf("\n\t**That person isn't in the table.**\n");
        return;
    }


    //compute the hash index, and store into 'index'
    int index = hash(person->name);

    //search the table for the index associated with the name and NULL-ify
    for(int i = 0; i < TABLESIZE; i++){

        int try_index = (index + i) % TABLESIZE;
        
        //You can safely ignore the DELETED_NODE address
        if(table[try_index] == DELETED_NODE){
            continue;
        }

        //NULL means that the computed try_index was never possibly inserted into the table, so break the function here
        if (table[try_index] == NULL){
            return;
        }


        //In the case where the encountered slot of the hash table is a linked list, meaning that external chaining has occurred here 
        /** @todo THIS METHOD only deletes the node if it is at the head. DOES NOT incorporate a linked list traversal if node is downstream.*/
        if(table[try_index]->name == person->name){

            //leader is required so as to not modify the current slot directly
            //Initially this was a problem when inserting person structs directly in the source code.
            struct person_t* leader = table[try_index]->next;
            while(leader){
                free(table[try_index]);
                table[try_index] = leader;
                leader = leader->next;
            }

            //does this struct, at table[try_index] need to be freed first????
            table[try_index] = DELETED_NODE;    //DELETED_NODE is the sentinel value
            //table[try_index]->next = NULL;
            return;
        }
    }
    
    //printf("Person not found in the array\n");
    return;
}



//find a person in the table by their name
struct person_t* find_person(struct person_t* *table){

    char name[MAX_NAME];
    printf("Who are you looking for?\n");
    fgets(name, MAX_NAME, stdin);
    sscanf(name, "%s", name);
    
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
    

    //compute the hash value
    int index = hash(person->name);
    //disallow the user to insert with the tail method when the current *(table+index) slot is NULL
    if(!(*(table + index))){
        printf("\n**Cannot insert a tail until a head is first placed.**\n");
        return;
    }

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
