// Copyright © 2024 soupglasses <sofi+git@mailbox.org>
// SPDX-License-Identifier: EUPL-1.2
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>

#include "list.h"

// A non zero return value means its equal.
int item_equal(item_t left, item_t right) {
  if (left.type != right.type) return 0;
  switch (left.type) {
    case 'i':
      return (left.data.i == right.data.i);
    case 'd':
      return (left.data.d == right.data.d);
    case 'c':
      return (left.data.c == right.data.c);
    case 's':
      return (strcmp(left.data.s, right.data.s) == 0);
    case 'p':
      return (left.data.p == right.data.p);
  };
  // Since we use a char, its technically possible to not match above.
  // TODO: Handle this better, dont just say its not equal.
  return 0;
}

void item_free(item_t elem) {
  switch (elem.type) {
    case 's':
      free(elem.data.s);
    case 'p':
      free(elem.data.p);
  };
}

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
node_t* list_elem_insert(node_t* left_elem, item_t elem) {
  node_t* current_elem = calloc(1, sizeof(node_t));
  if (current_elem == NULL) return NULL;
  node_t* right_elem = left_elem->next;

  current_elem->item = elem;

  current_elem->prev = left_elem;
  current_elem->next = right_elem;

  left_elem->next = current_elem;
  right_elem->prev = current_elem;

  return current_elem;
}

// Adds an element at the start of the list.
node_t* list_prepend(node_t* const sentinel, item_t elem) {
  // Recasting sentinel as its contract does not require the sentinel
  // as its argument (which is what const tries to convey).
  return list_elem_insert((node_t*) sentinel, elem);
}

// Adds an element at the end of the list.
node_t* list_append(node_t* const sentinel, item_t elem) {
  return list_elem_insert(sentinel->prev, elem);
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

node_t* list_find(node_t* const sentinel, item_t elem) {
  node_t* current = sentinel->next;
  while (current != sentinel) {
    if (item_equal(current->item, elem)) return current;
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
