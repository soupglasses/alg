// Copyright © 2024 soupglasses <sofi+git@mailbox.org>
//
// Licensed under the EUPL, with extension of article 5 (compatibility
// clause) to any licence for distributing derivative works that have
// been produced by the normal use of the Work as a library.

#ifndef SW2ALG_LIST_H_
#define SW2ALG_LIST_H_

#include "item.h"

typedef struct Node {
  item_t item;
  struct Node* prev;
  struct Node* next;
} node_t;

node_t* list_new(void);

void list_free(node_t* const sentinel);

int list_elem_is_sentinel(node_t* elem);
int list_empty(node_t* const sentinel);

node_t* list_elem_insert(node_t* left_elem, item_t item);
node_t* list_prepend(node_t* const sentinel, item_t item);
node_t* list_append(node_t* const sentinel, item_t item);

item_t list_elem_remove(node_t* elem);

node_t* list_at(node_t* const sentinel, int pos);
node_t* list_find(node_t* const sentinel, item_t item);
int list_length(node_t* const sentinel);

#endif //SW2ALG_LIST_H_
