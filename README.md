# Hash Table
This is a simple hash table implementation written in C.

## Structure

### **struct Hash_Table_Entry**
```c
char *key;
int value;
struct Hash_Table_Entry *next;
```

### **struct Hash_Table**
```c
Hash_Table_Entry **entries;
int tableSize;
```

## Methods
```c
Hash_Table *ht_create(int); // Creates a hash table with int entries.
unsigned int hash(Hash_Table *, const char *); // Hashes a key to the hash table.
void ht_set(Hash_Table *, char *, int); // Inserts a key, value pair into the table.
int ht_get(Hash_Table *, char *); // Returns the value of a given key from the table.
void ht_dump(Hash_Table*); // Printing the entire table.
void ht_free(Hash_Table*); // Cleanup.
```

## Todo
* Implement a delete method for removing a pair from the table.
