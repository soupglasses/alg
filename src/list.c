// Copyright © 2024 soupglasses <sofi+git@mailbox.org>
// SPDX-License-Identifier: EUPL-1.2
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "item.h"
#include "list.h"

// A circular doubly-linked list with a sentinel.
// Generated list should always point to the sentinel.
// Any item's defined as pointers/strings should be always heap allocated.
// Returns the sentinel position in the list. Do not change the pointer.
node_t* list_new(void) {
  node_t* sentinel = calloc(1, sizeof(node_t));
  if (sentinel == NULL) return NULL;

  // Create the sentinel, it's identfiable by being an item whose value
  // points to itself. By design it's not set to NULL, as the caller may
  // use this to represent an empty item in the list.
  sentinel->item.type = 'p';
  sentinel->item.data.p = sentinel;

  sentinel->prev = sentinel;
  sentinel->next = sentinel;

  return sentinel;
}

void list_free(node_t* const sentinel) {
  node_t* current = sentinel;
  node_t* next = current->next;
  current = next; // Skip the sentinel.
  while (current != sentinel) {
    next = current->next;
    item_free(current->item);
    free(current);
    current = next;
  }
  // Lastly, free our sentinel.
  free(sentinel);
}

int list_elem_is_sentinel(node_t* elem) {
  return elem->item.type == 'p' && elem->item.data.p == elem;
}

int list_empty(node_t* const sentinel) {
  return sentinel->prev == sentinel->next && list_elem_is_sentinel(sentinel);
}

// Insert to the right of the list element given.
// Returns the new list element which holds the new item.
// Could return NULL if Malloc fails.
node_t* list_elem_insert(node_t* left_elem, item_t item) {
  node_t* middle_elem = calloc(1, sizeof(node_t));
  if (middle_elem == NULL) return NULL;
  node_t* right_elem = left_elem->next;

  middle_elem->item = item;

  middle_elem->prev = left_elem;
  middle_elem->next = right_elem;

  left_elem->next = middle_elem;
  right_elem->prev = middle_elem;

  return middle_elem;
}

// Adds an element at the start of the list.
node_t* list_prepend(node_t* const sentinel, item_t item) {
  // Recasting sentinel as its contract does not require the sentinel
  // as its argument (which is what const tries to convey).
  return list_elem_insert((node_t*) sentinel, item);
}

// Adds an element at the end of the list.
node_t* list_append(node_t* const sentinel, item_t item) {
  return list_elem_insert(sentinel->prev, item);
}

// Caller must handle deallocating possible pointers inside returned item_t.
// May fail on empty list, returns ITEM_NULL that you should check for.
item_t list_elem_remove(node_t* elem) {
  if (list_elem_is_sentinel(elem)) return ITEM_NULL;

  node_t* left = elem->prev;
  node_t* right = elem->next;

  left->next = right;
  right->prev = left;

  item_t item = elem->item;

  free(elem);
  return item;
}

// Returns NULL if out of bounds.
node_t* list_at(node_t* const sentinel, int pos) {
  node_t* current = sentinel;
  if (list_empty(sentinel)) return NULL;
  if (pos == 0) return sentinel->next;
  if (pos < 0) {
    do {
      pos += 1;
      current = current->prev;
      if (current == sentinel) return NULL;
    } while(pos != 0);
  }
  if (pos > 0) {
    current = current->next;
    do {
      pos -= 1;
      current = current->next;
      if (current == sentinel) return NULL;
    } while(pos != 0);
  };
  return current;
}

node_t* list_find(node_t* const sentinel, item_t item) {
  node_t* current = sentinel->next;
  while (current != sentinel) {
    if (item_equal(current->item, item)) return current;
    current = current->next;
  }
  // We didn't find anything.
  return NULL;
}

int list_length(node_t* const sentinel) {
  node_t* cur = sentinel->next;
  int count = 0;
  while (cur != sentinel) {
    count += 1;
    cur = cur->next;
  };
  return count;
}
