// Copyright © 2024 soupglasses <sofi+git@mailbox.org>
//
// Licensed under the EUPL, with extension of article 5 (compatibility
// clause) to any licence for distributing derivative works that have
// been produced by the normal use of the Work as a library.

#ifndef SW2ALG_ITEM_H_
#define SW2ALG_ITEM_H_

#define ITEM_NULL (item_t) { .type = 'p', .data.p = NULL }

typedef struct Item {
  char type;
  union {
    int i;
    double d;
    char c;
    char* s;
    void* p;
  } data;
} item_t;

int item_equal(item_t left, item_t right);
void item_free(item_t item);

#endif //SW2ALG_ITEM_H_
