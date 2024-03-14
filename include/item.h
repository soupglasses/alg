// Copyright © 2024 soupglasses <sofi+git@mailbox.org>
// SPDX-License-Identifier: EUPL-1.2
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
void item_free(item_t elem);

#endif //SW2ALG_ITEM_H_
