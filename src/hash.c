// Copyright © 2024 soupglasses <sofi+git@mailbox.org>
// SPDX-License-Identifier: EUPL-1.2
#include <stdlib.h>
#include "hash.h"

// Implementing the hash function `djb2` as per described by Ozan Yigit
// at York University's Electrical Engineering and Computer Science
// Department. It has good distribution of keys and is relatively simple
// to implement, which is why it has been chosen.
//
// Source: http://www.cse.yorku.ca/~oz/hash.html
unsigned long djb2_hash(const char* str) {
  const unsigned char* ustr = (const unsigned char*) str;
  unsigned long hash = 5381u;
  int c;

  while ((c = *ustr++))
    /* equivalent to: hash * 33 + c */
    hash = ((hash << 5) + hash) + c;

  return hash;
}

hash_t* hash_new(unsigned int size) {
  hash_t* hash_table = calloc(1, sizeof(hash_t));
  if (hash_table == NULL)
    goto malloc_fail;

  hash_table->size = size;
  hash_table->keys = calloc(size, sizeof(unsigned long));
  hash_table->values = calloc(size, sizeof(void*));
  if (hash_table->keys == NULL || hash_table->values == NULL)
    goto malloc_fail;

  for (unsigned int i = 0; i < size; i++)
    hash_table->keys[i] = 0;
  return hash_table;

  malloc_fail:
  if(hash_table)
    hash_free(hash_table);
  return NULL;
}

// NOTE: We do not free any values, just the array holding the pointers
// to the valuees. This is because we cannot know what values were
// possibly allocated dynamically.
void hash_free(hash_t* hash_table) {
  free(hash_table->values);
  free(hash_table->keys);
  free(hash_table);
}

int hash_index(const hash_t* hash_table, const char* key) {
  unsigned long key_hash = djb2_hash(key);
  int i = key_hash % hash_table->size;
  // Avoid case of infinite looping when hash table is full and asking
  // for non-existent key.
  int attempts = hash_table->size;
  while(attempts != 0 && hash_table->keys[i] != 0 && hash_table->keys[i] != key_hash) {
    // Linear probing, as we hit a valid key which is not ours.
    i = (i + 1) % hash_table->size;
    attempts -= 1;
  }
  if (attempts == 0) return -1;
  return i;
}

// Return a value found at position key.
void* hash_get(const hash_t* hash_table, const char* key) {
  int i = hash_index(hash_table, key);
  if (i == -1) return NULL;
  return hash_table->values[i];
}

// Set and return the value at position key.
void* hash_put(hash_t* hash_table, const char* key, void* value) {
  int i = hash_index(hash_table, key);
  if (i == -1) return NULL;

  hash_table->keys[i] = djb2_hash(key);
  hash_table->values[i] = value;

  return hash_table->values[i];
}

// Remove and return the value at position key.
void* hash_remove(hash_t* hash_table, const char* key) {
  int i = hash_index(hash_table, key);
  if (i == -1) return NULL;
  hash_table->keys[i] = 0;
  void* value = hash_table->values[i];
  hash_table->values[i] = NULL;
  return value;
}

// Returns the count of set keys in the hash table.
int hash_count(hash_t* hash_table) {
  int count = 0;
  for(unsigned int i = 0; i < hash_table->size; i++)
    if (hash_table->keys[i] != 0)
      count += 1;

  return count;
}
