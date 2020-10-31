/**************************************************************
* Written by Andrei Georgescu (Fall 2020)
* hash_table.c
*
* This is the implementation for hash_table.
* Hash_Table uses a hash function to map a unique
* number (hash) to a key. This allows for really fast
* lookup, especially if no collissions occur.     
**************************************************************/
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include "hash_table.h"

/**
 *  Creates a hash table and allocates memory for each entry
 *  in the table.
 * 
 *  @param tableSize the amount of rows to be allocated
 *  @return 1. pointer to an allocated hash table with tableSize rows
 *          2. NULL if an error occurred during allocation.
 */
Hash_Table *ht_create(int tableSize) {
    // Allocating the hash table
    Hash_Table *hash_table = malloc(sizeof(Hash_Table) * 1);
    if(hash_table == NULL) return NULL;

    hash_table->tableSize = tableSize;

    // Allocating the hash table entries
    hash_table->entries = malloc(sizeof(Hash_Table_Entry*) * tableSize);
    if(hash_table->entries == NULL) return NULL;
    
    // Initializing entries
    for(int i = 0; i < tableSize; i++) {
        hash_table->entries[i] = NULL;
    }

    return hash_table;
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
 *  This is a helper function responsible for hashing a key to 
 *  specific index within the hash table.
 * 
 *  NOTE: All keys are hashed to lowercase. This means that
 *        "t" and "T" are considered the same key and would
 *        have the same value in the table.
 * 
 *  @param hash_table the source hash table
 *  @param key the key to be used for the hash
 *  @return unique integer that represents a case-insensitive version of key
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

    // Cleanup
    free(lowerKey);

    // Ensuring that value is <= 0 && < TABLE_SIZE
    return value % hash_table->tableSize;
}

/**
 *  Inserts a Key, Value pair into a hash table.
 * 
 *  @param hash_table the source hash table
 *  @param key the key of the pair
 *  @param value the value of the pair
 */ 
void ht_set(Hash_Table *hash_table, char *key, const int value) {
    // Getting hash and locating the entry.
    unsigned int index = hash(hash_table, key);
    Hash_Table_Entry *entry = hash_table->entries[index];

    // If no entry with this key exists, we can insert it into the table.
    if(entry == NULL) {
        hash_table->entries[index] = pair(key, value);
        return;
    }

    // Otherwise, we need to update the value or handle a collission.
    Hash_Table_Entry *prev_entry;
    // Looping through all Hash_Table_Entries with the same hash.
    while(entry != NULL) {
        // Found a key match, replacing its value.
        if(strcasecmp(entry->key, key) == 0) {
            entry->value = value;
            return;
        }

        prev_entry = entry;
        entry = prev_entry->next;
    }

    // If we've reached this point, a collission has occurred so
    // we can just append the pair to the entry's next pointer.
    prev_entry->next = pair(key, value);
}

/**
 *  Returns the value associated with the given key.
 *
 *  @param hash_table the source hash table
 *  @param key the key of the pair
 *  @return 1. the value associated with the given key.
 *          2. 0 if the key is not in the table.
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
 *  Prints all Key, Value pairs in the hash table.
 */ 
void ht_dump(Hash_Table *hash_table) {
    // Looping through all entries in the hash table.
    for(int i = 0; i < hash_table->tableSize; i++) {
        Hash_Table_Entry *entry = hash_table->entries[i];
        if(entry == NULL) continue;

        printf("[%d]: ", i);

        // Looping through all entries and their collisions.
        for(;;) {
            printf("{ %s, %d }", entry->key, entry->value);

            // If we have reached the end of the collision, break.
            if(entry->next == NULL) break;
            entry = entry->next;
        }

        printf("\n");
    }
}

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

// /**
//  *  Calculates the top entries by value in a hash table.
//  *  
//  *  sorted is an array of pointers to Hash_Table_Entry structs that are
//  *  sorted in order by their value after this function is done executing. 
//  * 
//  *  @param hash_table the source hash table
//  *  @param sorted An allocated array of Hash_Table_Entry pointers
//  *  @param topAmount the amount of entries to consider
//  */ 
// void ht_get_top_entries(Hash_Table* hash_table, Hash_Table_Entry **sorted, int topAmount) {    
//     for(int i = 0; i < hash_table->tableSize; i++) {
//         Hash_Table_Entry *entry = hash_table->entries[i];
//         if(entry == NULL) continue;

//         // Looping through all entries and their collisions.
//         for(;;) {

//             // Looping through the sorted entries to compare its values.
//             for(int j = 0; j < topAmount; j++) {
//                 // The value of the current element in sorted is smaller
//                 // then the entry it is being compared to.
//                 if(sorted[j]->value < entry->value) {
                
//                     // Looping through the sorted array backwards and shifting
//                     // each element to the right by one.
//                     for(int k = topAmount; k > j; k--) {
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