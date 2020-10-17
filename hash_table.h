/**************************************************************
* Class: CSC-415-01 Fall 2020
* Name: Andrei Georgescu
* Student ID: 920776919
* GitHub ID: Doxify
* Project: Assignment 4 – Word Blast
*
* File: hash_table.h
*
* Description: This is the header file for the hash_table. 
*
**************************************************************/
#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>

// Each hash table has a Hash_Table_Entry which is responsible for storing
// a key, value pair and a pointer to the next entry in case of collissions.
typedef struct Hash_Table_Entry {
    char *key;
    int value;
    struct Hash_Table_Entry *next;
} Hash_Table_Entry;

typedef struct Hash_Table {
    pthread_t *threads;
    Hash_Table_Entry **entries;
    int tableSize;
} Hash_Table;

Hash_Table *ht_create(int);
unsigned int hash(Hash_Table *, const char *);
void ht_set(Hash_Table *, char *, int);
int ht_get(Hash_Table *, char *);
void ht_get_top_entries(Hash_Table*, Hash_Table_Entry **, int);   
void ht_free(Hash_Table*);

#endif