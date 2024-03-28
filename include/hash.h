// Copyright © 2024 soupglasses <sofi+git@mailbox.org>
//
// Licensed under the EUPL, with extension of article 5 (compatibility
// clause) to any licence for distributing derivative works that have
// been produced by the normal use of the Work as a library.

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

int _hash_desired_index(const hash_t* hash_table, const char* key);
int hash_index(const hash_t* hash_table, const char* key);

void* hash_get(const hash_t* hash_table, const char* key);
void* hash_put(hash_t* hash_table, const char* key, void* value);
void* hash_remove(hash_t* hash_table, const char* key);

int hash_count(hash_t* hash_table);

#endif //SW2ALG_HASH_H_
