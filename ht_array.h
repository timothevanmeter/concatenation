// Simple hash table implemented in C.

#ifndef _HT_H
#define _HT_H

#include <stdbool.h>
#include <stddef.h>

// Hash table structure: create with ht_create, free with ht_destroy.
typedef struct ht ht;

// Create hash table and return pointer to it, or NULL if out of memory.
ht* ht_create(void);

// Free memory allocated for hash table, including allocated keys.
void ht_destroy(ht* table);

// Get item with given key (NUL-terminated) from hash table. Return
// value (which was set with ht_set), or NULL if key not found.
void* ht_get(ht* table, char* key);

// Adds a new count for a given key entry. This function is
//  specific to the customize version ht_array
void add_count(int** counts, int new_count);

// Set item with given key (NUL-terminated) to value (which must not
// be NULL). If not already present in table, key is copied to newly
// allocated memory (keys are freed automatically when ht_destroy is
// called). Return address of copied key, or NULL if out of memory.
char* ht_set(ht* table, char* key, int* count, int file_number);

// Return number of items in hash table.
size_t ht_length(ht* table);

// Hash table iterator: create with ht_iterator, iterate with ht_next.
typedef struct {
    char* key;  // current key
    int* counts;      // current value

    // Don't use these fields directly.
    ht* _table;       // reference to hash table being iterated
    size_t _index;    // current index into ht._entries
} hti;

// Return new hash table iterator (for use with ht_next).
hti ht_iterator(ht* table);

// Move iterator to next item in hash table, update iterator's key
// and value to current item, and return true. If there are no more
// items, return false. Don't call ht_set during iteration.
bool ht_next(hti* it);

#endif // _HT_H
