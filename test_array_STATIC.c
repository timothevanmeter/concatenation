#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
#include <assert.h>
#include "ht_test.h"

#define FILE_NUMBER 10
#define INITIAL_CAPACITY 16

typedef struct {
  char* key;  // key is NULL if this slot is empty
  int counts[FILE_NUMBER];
} ht_entry;

// Hash table structure: create with ht_create, free with ht_destroy.
struct ht {
  ht_entry* entries;  // hash slots
  size_t capacity;    // size of _entries array
  size_t length;      // number of items in hash table
};

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
    free((void*)table->entries[i].key);
    /* NO ALLOCATION FOR COUNTS SO NO NEED TO FREE */
    /* if(table->entries[i].counts != NULL) { */
    /*   free(table->entries[i].counts); */
    /* } */
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

/* ------------------------------------------------- */
void add_count(int *counts, int* new) {
  int i = 0;
  while( counts[i] != 0 ) {
    /* printf( "%d\n", (*temp) ); */
    i++;
  }
  counts[i] = (*new);
  fprintf( stdout," %d\n", counts[i] );
  fflush(stdout);
}
/* ------------------------------------------------- */

// Internal function to set an entry (without expanding table).
char* ht_set_entry(ht_entry* entries, size_t capacity,
        char* key, int* count, size_t* plength) {
    // AND hash with capacity-1 to ensure it's within entries array.
    uint64_t hash = hash_key(key);
    size_t index = (size_t)(hash & (uint64_t)(capacity - 1));

    /* fprintf(stdout, "---\n"); */
    /* fflush(stdout); */
    // Loop till we find an empty entry.
    while (entries[index].key != NULL) {
        if (strcmp(key, entries[index].key) == 0) {
	  // Found key (it already exists), update value.

	  fprintf(stdout, "%d :", (*count));
	  fflush(stdout);
	  add_count(entries[index].counts, count);

	  /* entries[index].counts = temp; */
	  /* fprintf(stdout, "%d\n---\n", (*entries[index].counts)); */
	  /* fflush(stdout); */
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
    entries[index].key = (char*)key;
    /* int *temp = entries[index].counts; */

    add_count(entries[index].counts, count);

    /* entries[index].counts = temp; */
    fprintf(stdout, "%d\n---\n", (*entries[index].counts));
    fflush(stdout);
    
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
                         entry.counts, NULL);
        }
    }

    // Free old entries array and update this table's details.
    free(table->entries);
    table->entries = new_entries;
    table->capacity = new_capacity;
    return true;
}

char* ht_set(ht* table, char* key, int* count) {
  assert(count != NULL);
  if (count == NULL) {
    fprintf(stdout,"*");
    fflush(stdout);
    return NULL;
  }

  // If length will exceed half of current capacity, expand it.
  if (table->length >= table->capacity / 2) {
    if (!ht_expand(table)) {
      fprintf(stdout,"*");
      fflush(stdout);    
      return NULL;
    }
  }

  /* fprintf(stdout, "\n##\n"); */
  /* fflush(stdout); */
  // Set entry and update length.
  return ht_set_entry(table->entries, table->capacity, key, count,
		      &table->length);
}

size_t ht_length(ht* table) {
    return table->length;
}

/* ------------------------------------------------- */

void add( int ** array, int new ) {
  int * temp = (*array);
  while( (*temp) != 0 ) {
    /* printf( "%d\n", (*temp) ); */
    temp++;
  }
  (*temp) = new;
  /* printf( "%d\n", (*temp) ); */
  array = &temp;
}

// ###########################################################################

int main(int argc, char **argv)
{
  int * array = calloc( FILE_NUMBER, sizeof(long int) );
  int ** ar = &array;  
  
  printf( "\n" );
  
  array[0] = 1;
  for(int i=1; i < FILE_NUMBER-3; i++) {
    array[i] = array[i-1] * i;
  }
  /* for(int i=0; i < FILE_NUMBER; i++) { */
  /*   printf( "%d\n", array[i] ); */
  /* } */

  /* printf( "\n--------------------\n" ); */
  add( ar , 142656731 );
  add( ar , 13 );
  add( ar , 14 );
  printf( "\n--------------------\n" );
  for(int i=0; i < FILE_NUMBER; i++) {
    printf( "%d\n", array[i] );
  }
  printf( "--------------------\n" );
  /* ----------------------------------------------- */
  ht *table = ht_create();
  char *k1 = "AA";
  /* int c1 = 123; */
  /* int *p1 = &c1; */
  for(int i=0; i < FILE_NUMBER; i++) {
    int *temp = &array[i];
    printf("%d : ", (*temp));
    ht_set(table, k1, temp);    
  }
  /* for(int i=0; i < FILE_NUMBER; i++) { */
  /*   printf( "%d\n",  ); */
  /* } */

  ht_destroy(table);
  free(array);
  
  return 0;
}
