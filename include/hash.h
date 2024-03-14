// Copyright © 2024 soupglasses <sofi+git@mailbox.org>
// SPDX-License-Identifier: EUPL-1.2
#ifndef SW2ALG_HASH_H_
#define SW2ALG_HASH_H_

typedef struct Hash {
  unsigned int size;
  unsigned long* keys;
  void** values;
} hash_t;

unsigned long djb2_hash(const char* str);

hash_t* hash_new(unsigned int size);
void hash_free(hash_t* hash_table);

int hash_index(const hash_t* hash_table, const char* key);

void* hash_get(const hash_t* hash_table, const char* key);
void* hash_put(hash_t* hash_table, const char* key, void* value);
void* hash_remove(hash_t* hash_table, const char* key);

int hash_count(hash_t* hash_table);

#endif //SW2ALG_HASH_H_
