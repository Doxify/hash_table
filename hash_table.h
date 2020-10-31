/**************************************************************
* Written by Andrei Georgescu (Fall 2020)
* hash_table.h
*
* This is the header file for the hash_table. 
**************************************************************/
#ifndef HASH_TABLE_H
#define HASH_TABLE_H

// Key, Value pair struct that also handles collissions.
typedef struct Hash_Table_Entry {
    char *key;
    int value;
    struct Hash_Table_Entry *next;
} Hash_Table_Entry;

// Hash Table struct which stores key, value entries and
// metadata about the table.
typedef struct Hash_Table {
    Hash_Table_Entry **entries;
    int tableSize;
} Hash_Table;

Hash_Table *ht_create(int); // Creates a hash table with int entries.
unsigned int hash(Hash_Table *, const char *); // Hashes a key to the hash table.
void ht_set(Hash_Table *, char *, int); // Inserts a key, value pair into the table.
int ht_get(Hash_Table *, char *); // Returns the value of a given key from the table.
void ht_dump(Hash_Table*); // Printing the entire table.
void ht_free(Hash_Table*); // Cleanup.
// void ht_get_top_entries(Hash_Table*, Hash_Table_Entry **, int);

#endif