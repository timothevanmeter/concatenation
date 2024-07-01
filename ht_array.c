#include "ht_array.h"
#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// Hash table entry (slot may be filled or empty).
typedef struct {
  char* key;  // key is NULL if this slot is empty
  int* counts;
} ht_entry;

// Hash table structure: create with ht_create, free with ht_destroy.
struct ht {
  ht_entry* entries;  // hash slots
  size_t capacity;    // size of _entries array
  size_t length;      // number of items in hash table
};

#define INITIAL_CAPACITY 16  // must not be zero

ht* ht_create(void) {
  // Allocate space for hash table struct.
  ht* table = malloc(sizeof(ht));
  if (table == NULL) {
    return NULL;
  }
  table->length = 0;
  table->capacity = INITIAL_CAPACITY;

  // Allocate (zero'd) space for entry buckets.
  table->entries = calloc(table->capacity, sizeof(ht_entry));
  if (table->entries == NULL) {
    free(table); // error, free table before we return!
    return NULL;
  }
  return table;
}

void ht_destroy(ht* table) {
  // First free allocated keys.
  for (size_t i = 0; i < table->capacity; i++) {
    free(table->entries[i].key);
    free(table->entries[i].counts);
  }

  // Then free entries array and table itself.
  free(table->entries);
  free(table);
}

#define FNV_OFFSET 14695981039346656037UL
#define FNV_PRIME 1099511628211UL

// Return 64-bit FNV-1a hash for key (NUL-terminated). See description:
// https://en.wikipedia.org/wiki/Fowler–Noll–Vo_hash_function
static uint64_t hash_key(char* key) {
  uint64_t hash = FNV_OFFSET;
  for (char* p = key; *p; p++) {
    hash ^= (uint64_t)(unsigned char)(*p);
    hash *= FNV_PRIME;
  }
  return hash;
}

void* ht_get(ht* table, char* key) {
  // AND hash with capacity-1 to ensure it's within entries array.
  uint64_t hash = hash_key(key);
  size_t index = (size_t)(hash & (uint64_t)(table->capacity - 1));

  // Loop till we find an empty entry.
  while (table->entries[index].key != NULL) {
    if (strcmp(key, table->entries[index].key) == 0) {
      // Found key, return value.
      return table->entries[index].counts;
    }
    // Key wasn't in this slot, move to next (linear probing).
    index++;
    if (index >= table->capacity) {
      // At end of entries array, wrap around.
      index = 0;
    }
  }
  return NULL;
}

/* ------------------------------------------------- */
void add_count(int** counts, int new_count) {
  int i = 0;
  while ((*counts) != NULL) {
    (*counts)++;
    i++;
  }
  int* new = malloc((i + 1) * sizeof(int)); // +2 for the new count and zero terminator
  if (new == NULL) {
    return;
  }    
  memcpy(new, *counts, i * sizeof(int));
  new[i] = new_count;    
  free(*counts);
  *counts = new;
}
/* ------------------------------------------------- */
/* void create_array(ht_entry *entry, int file_number, int *count) { */
/*   int *array = malloc(file_number * sizeof(int)); */
/*   /\* int **a = &array; *\/ */
/*   int i = 0; */
/*   while(i != file_number-1) { */
/*     array[i] = 0; */
/*   } */
/*   array[i] = *count; */
/*   entry->counts = array; */
/*   /\* return a; *\/ */
/* } */
/* ------------------------------------------------- */

// Internal function to set an entry (without expanding table).
char* ht_set_entry(ht_entry* entries, size_t capacity,
		   char* key, int* count, size_t* plength, int file_number) {
  // AND hash with capacity-1 to ensure it's within entries array.
  uint64_t hash = hash_key(key);
  size_t index = (size_t)(hash & (uint64_t)(capacity - 1));
  
  // Loop till we find an empty entry.
  while (entries[index].key != NULL) {
    if (strcmp(key, entries[index].key) == 0) {
      fprintf(stdout,"\n*** = %s\n", entries[index].key);
      fflush(stdout);
      // Found key (it already exists), update value.
      add_count(&entries[index].counts, (*count));
      return entries[index].key;
    }
    // Key wasn't in this slot, move to next (linear probing).
    index++;
    if (index >= capacity) {
      // At end of entries array, wrap around.
      index = 0;
    }
  }

  // Didn't find key, allocate+copy if needed, then insert it.
  if (plength != NULL) {
    key = strdup(key);
    if (key == NULL) {
      return NULL;
    }
    (*plength)++;
  }
  entries[index].key = key;
  /* create_array(&entries[index], file_number, count); */
  entries[index].counts = calloc(file_number, sizeof(int));
  entries[index].counts[file_number] = *count;
  /* entries[index].counts = create_array(file_number, count)); */
  return key;
}

// Expand hash table to twice its current size. Return true on success,
// false if out of memory.
static bool ht_expand(ht* table) {
  // Allocate new entries array.
  size_t new_capacity = table->capacity * 2;
  if (new_capacity < table->capacity) {
    return false;  // overflow (capacity would be too big)
  }
  ht_entry* new_entries = calloc(new_capacity, sizeof(ht_entry));
  if (new_entries == NULL) {
    return false;
  }

  // Iterate entries, move all non-empty ones to new table's entries.
  for (size_t i = 0; i < table->capacity; i++) {
    ht_entry entry = table->entries[i];
    if (entry.key != NULL) {
      ht_set_entry(new_entries, new_capacity, entry.key,
		   entry.counts, NULL, 0);
    }
    free(table->entries[i].counts);
  }

  // Free old entries array and update this table's details.
  free(table->entries);
  table->entries = new_entries;
  table->capacity = new_capacity;
  return true;
}

char* ht_set(ht* table, char* key, int* count, int file_number) {
  assert(count != NULL);
  if (count == NULL) {
    return NULL;
  }

  // If length will exceed half of current capacity, expand it.
  if (table->length >= table->capacity / 2) {
    if (!ht_expand(table)) {
      return NULL;
    }
  }

  // Set entry and update length.
  return ht_set_entry(table->entries, table->capacity, key, count,
		      &table->length, file_number);
}

size_t ht_length(ht* table) {
  return table->length;
}

hti ht_iterator(ht* table) {
  hti it;
  it._table = table;
  it._index = 0;
  return it;
}

bool ht_next(hti* it) {
  // Loop till we've hit end of entries array.
  ht* table = it->_table;
  while (it->_index < table->capacity) {
    size_t i = it->_index;
    it->_index++;
    if (table->entries[i].key != NULL) {
      // Found next non-empty item, update iterator key and counts.
      ht_entry entry = table->entries[i];
      it->key = entry.key;
      it->counts = entry.counts;
      return true;
    }
  }
  return false;
}
