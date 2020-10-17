/**************************************************************
* Class: CSC-415-01 Fall 2020
* Name: Andrei Georgescu
* Student ID: 920776919
* GitHub ID: Doxify
* Project: Assignment 4 – Word Blast
*
* File: hash_table.c
*
* Description: This is the implementation for hash_table.
*              Hash_Table uses a hash function to map a unique
*              number (hash) to a key. This allows for really fast
*              lookup, especially if no collissions occur.
*              
*
**************************************************************/
#include "hash_table.h"

/**
 *  Creates a hash table and allocates memory for each entry
 *  in the table.
 * 
 *  @param tableSize the amount of rows to be allocated
 *  @return a pointer to an allocated hash table with tableSize rows.
 */
Hash_Table *ht_create(int tableSize) {
    // Allocating the hash table
    Hash_Table *hash_table = malloc(sizeof(Hash_Table) * 1);
    hash_table->tableSize = tableSize;
    
    // Allocating the hash table entries
    hash_table->entries = malloc(sizeof(Hash_Table_Entry*) * tableSize);

    // Initializing entries
    for(int i = 0; i < tableSize; i++) {
        hash_table->entries[i] = NULL;
    }

    return hash_table;
}

/**
 *  The hash function is responsible for hashing a key to specific
 *  location within the hash table.
 * 
 *  NOTE: All keys are hasehed to lowercase.
 * 
 *  @param hash_table the source hash table
 *  @param key the key to be used for the hash
 *  @return a unique hash that represents a case-sensitive version of key
 */ 
unsigned int hash(Hash_Table *hash_table, const char *key) {
    // Converting the key to lowercase
    char *lowerKey = (char*)malloc(strlen(key) + 1);
    strcpy(lowerKey, key);
    for(int i = 0; i < lowerKey[i] != '\0'; i++) {
        lowerKey[i] = tolower(lowerKey[i]);
    }

    // Creating a hash based on the lowercase version of the string.
    unsigned long int value = 0;
    for(int i = 0; i < lowerKey[i] != '\0'; i++) {
        value = 35 * value + lowerKey[i];
    }

    free(lowerKey);

    // Ensuring that value is <= 0 && < TABLE_SIZE
    return value % hash_table->tableSize;
}

/**
 *  This is a helper function that allocates a Hash_Table_Entry.
 * 
 *  @param key the key of the pair
 *  @param value the value of the pair
 *  @return an allocated Pair with the given key and value.
 */ 
Hash_Table_Entry *pair(char *key, const int value) {
    // Allocating memory for the entry.
    Hash_Table_Entry *entry = malloc(sizeof(Hash_Table_Entry) * 1);
    entry->key = malloc(strlen(key) + 1);

    // Coyping the key and value into the entry.
    strcpy(entry->key, key);
    entry->value = value;

    // Setting next pointer to null
    entry->next = NULL;

    return entry;
}

/**
 *  Adds a Hash_Table_Entry into the hash table.
 * 
 *  @param hash_table the source hash table
 *  @param key the key of the pair
 *  @param value the value of the pair
 */ 
void ht_set(Hash_Table *hash_table, char *key, const int value) {
    // Getting hash and locating the entry.
    unsigned int index = hash(hash_table, key);
    Hash_Table_Entry *entry = hash_table->entries[index];

    // If no entry with this key exists, simply insert it into the table.
    if(entry == NULL) {
        hash_table->entries[index] = pair(key, value);
        return;
    }

    // If an entry exists, we either need to update the value or handle
    // a collission.
    Hash_Table_Entry *prev_entry;
    while(entry != NULL) {
        // Found a match, replace it's value.
        if(strcasecmp(entry->key, key) == 0) {
            entry->value = value;
            return;
        }

        prev_entry = entry;
        entry = prev_entry->next;
    }

    // If we've reached this point, a collission has occurred so
    // we must append the entry's next pointer.
    prev_entry->next = pair(key, value);
}

/**
 *  Returns the value associated with the given key.
 *  
 *  If no key exists in the hash table, 0 is returned.
 * 
 *  @param hash_table the source hash table
 *  @param key the key of the pair
 *  @return the value associated with the given key.
 */ 
int ht_get(Hash_Table *hash_table, char *key) {
    // Getting hash and locating the entry.
    unsigned int index = hash(hash_table, key);
    Hash_Table_Entry *entry = hash_table->entries[index];

    // Entry does not exist.
    if(entry == NULL) {
        return 0;
    }

    // Going through all entries at the index.
    while(entry != NULL) {
        // If the key has been found, return it's value.
        if(strcasecmp(entry->key, key) == 0) {
            return entry->value;
        }

        entry = entry->next;
    }

    // If we've reached this point, there were more than one entries
    // at the given index, but none matched the provided key.
    return 0;
}

/**
 *  Calculates the top entries by value in a hash table.
 *  
 *  sorted is an array of pointers to Hash_Table_Entry structs that are
 *  sorted in order by their value after this function is done executing. 
 * 
 *  @param hash_table the source hash table
 *  @param sorted An allocated array of Hash_Table_Entry pointers
 *  @param topAmount the amount of entries to consider
 */ 
void ht_get_top_entries(Hash_Table* hash_table, Hash_Table_Entry **sorted, int topAmount) {    
    for(int i = 0; i < hash_table->tableSize; i++) {
        Hash_Table_Entry *entry = hash_table->entries[i];
        if(entry == NULL) continue;

        // Looping through all entries and their collisions.
        for(;;) {

            // Looping through the sorted entries to compare its values.
            for(int j = 0; j < topAmount; j++) {
                // The value of the current element in sorted is smaller
                // then the entry it is being compared to.
                if(sorted[j]->value < entry->value) {
                
                    // Looping through the sorted array backwards and shifting
                    // each element to the right by one.
                    for(int k = topAmount; k > j; k--) {
                        sorted[k] = sorted[k - 1];
                    }
                    
                    // Replacing the sorted entry with the current entry after
                    // the sorted array has been shifted over.
                    sorted[j] = entry;
                    break;
                }
            }

            // If we have reached the end of the collision, break.
            if(entry->next == NULL) break;
            entry = entry->next;
        }
    }
}


/**
 *  NOTE: This was my attempt at splitting the sorting process and using threads
 *        to sort chunks of the hash table. I found that it didn't improve the
 *        runtime at all when I used it so I scrapped it and went back to sorting
 *        the hash table all in one go.
 * /
// void *processThreads(void *arg) {
//     Hash_Table_Args args = *(Hash_Table_Args*) arg;
//     Hash_Table *hash_table = args.hash_table;
//     Hash_Table_Entry **sorted = args.sorted;
//     unsigned int processSize = args.hash_table->tableSize / hash_table->threadCount;
//     unsigned int remainder = args.hash_table->tableSize - (processSize * hash_table->threadCount);
    
//     int prevOffset = processSize * (args.num - 1);
//     int offset = processSize * args.num;

//     // Accouting for the remainder here.
//     if (args.num == hash_table->threadCount) {
//         offset += remainder;
//     }

//     printf("prev: %d, offset: %d\n", prevOffset, (offset == 0 ? 0 : offset - 1));

//     for(int i = prevOffset; i < offset; ++i) {
//         Hash_Table_Entry *entry = hash_table->entries[i];
//         if(entry == NULL) continue;

//         // Looping through all entries and their collisions.
//         for(;;) {

//             // Looping through the sorted entries to compare its values.
//             for(int j = 0; j < args.topAmount; j++) {
//                 // The value of the current element in sorted is smaller
//                 // then the entry it is being compared to.
//                 if(sorted[j]->value < entry->value) {
                
//                     // Looping through the sorted array backwards and shifting
//                     // each element to the right by one.
//                     for(int k = args.topAmount; k > j; k--) {
//                         sorted[k] = sorted[k - 1];
//                     }
                    
//                     // Replacing the sorted entry with the current entry after
//                     // the sorted array has been shifted over.
//                     sorted[j] = entry;
//                     break;
//                 }
//             }

//             // If we have reached the end of the collision, break.
//             if(entry->next == NULL) break;
//             entry = entry->next;
//         }
//     }

// }

// Hash_Table_Entry **init_top_entries_with_threads(Hash_Table *hash_table, int topAmount) {
    
//     // Creating all threads.
//     for(int i = 0; i < hash_table->threadCount; i++) {
//         // Allocating an array of pointers that Hash_Table_Args will use to store
//         // topAmount of pointers to Hash_Table_Entries.
//         Hash_Table_Entry **sorted = malloc(sizeof(Hash_Table_Entry*) * topAmount);
//         for(int i = 0; i < topAmount; i++) {
//             sorted[i] = malloc(sizeof(Hash_Table_Entry*));
//         }

//         // Configuring the args for each chunk of the process.
//         Hash_Table_Args *args = malloc(sizeof(Hash_Table_Args*));
//         args->hash_table = hash_table;
//         args->num = i + 1;
//         args->sorted = sorted;
//         args->topAmount = 10;
//         hash_table->args[i] = args;

//         pthread_create(&hash_table->threads[i], NULL, processThreads, args);
//     }

//     // Joining all threads.
//     for(int i = 0; i < hash_table->threadCount; i++) {
//         pthread_join(hash_table->threads[i], NULL);
//     }

//     return ht_combine_sorted_lists();

// };

/**
 *  Frees all memory allocated by the hash table.
 * 
 *  @param hash_table the hash table to remove from memory.
 */ 
void ht_free(Hash_Table *hash_table) {
    // Looping through all entries in the hash table.
    for(int i = 0; i < hash_table->tableSize; i++) {
        Hash_Table_Entry *entry = hash_table->entries[i];
        if(entry == NULL) continue;

        // Looping through all entries and their collisions.
        for(;;) {

            // Freeing the key and the entry itself.
            free(entry->key);
            free(entry);

            // If we have reached the end of the collision, break.
            if(entry->next == NULL) break;
            entry = entry->next;
        }
    }

    // Freeing the hash table itself.
    free(hash_table->entries);
    free(hash_table);
}