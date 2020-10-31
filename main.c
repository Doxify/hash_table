#include "hash_table.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    
    if (argc != 2) {
        printf("Error: Incorrect ussage\n");
        printf("only one run arg of table size is accepted.\n");
        return -1;
    }

    Hash_Table *ht = ht_create(atoi(argv[1]));
    if(ht == NULL) {
        printf("Error occurred while creating hash table.");
        return -1;
    }

    ht_set(ht, "test", 1);
    ht_set(ht, "TEST", 100);
    ht_set(ht, "test2", 111);
    ht_dump(ht);
    ht_free(ht);

    return 0;
}