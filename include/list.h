// Copyright © 2024 soupglasses <sofi+git@mailbox.org>
// SPDX-License-Identifier: EUPL-1.2
#ifndef SW2ALG_LIST_H_
#define SW2ALG_LIST_H_

#include "item.h"

typedef struct Node {
  item_t item;
  struct Node* prev;
  struct Node* next;
} node_t;

int item_equal(item_t left, item_t right);
void item_free(item_t elem);

node_t* list_new(void);

void list_free(node_t* const sentinel);

int list_elem_is_sentinel(node_t* elem);
int list_empty(node_t* const sentinel);

node_t* list_elem_insert(node_t* left_elem, item_t elem);
node_t* list_prepend(node_t* const sentinel, item_t elem);
node_t* list_append(node_t* const sentinel, item_t elem);

item_t list_elem_remove(node_t* elem);

node_t* list_at(node_t* const sentinel, int pos);
node_t* list_find(node_t* const sentinel, item_t elem);
int list_length(node_t* const sentinel);

#endif //SW2ALG_LIST_H_
