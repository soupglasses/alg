// Copyright © 2024 soupglasses <sofi+git@mailbox.org>
// SPDX-License-Identifier: EUPL-1.2
#include "mtest.h"
#include "item.h"
#include "list.h"

TEST_CASE(create_and_free, {
  node_t* const list = list_new();

  // Ensure sentinel is properly configured.
  CHECK_TRUE(list->next == list);
  CHECK_TRUE(list->prev == list);
  REQUIRE_TRUE(list->item.type == 'p');
  CHECK_TRUE(list->item.data.p == list);

  list_free(list);
})

TEST_CASE(insert, {
  node_t* const list = list_new();
  list_elem_insert((node_t*) list, (item_t) { .type = 'i', .data.i = 42 });
  CHECK_EQ_INT(list->next->item.data.i, 42);
  CHECK_EQ_INT(list->prev->item.data.i, 42);

  list_elem_insert((node_t*) list, (item_t) { .type = 'd', .data.d = 4.2 });
  CHECK_EQ_CHAR(list->next->item.type, 'd');
  CHECK_EQ_DOUBLE(list->next->item.data.d, 4.2, 0.001);

  CHECK_EQ_CHAR(list->prev->item.type, 'i');
  CHECK_EQ_INT(list->prev->item.data.i, 42);

  CHECK_EQ_CHAR(list->next->next->item.type, 'i');
  CHECK_EQ_INT(list->next->next->item.data.i, 42);
  CHECK_EQ_CHAR(list->prev->prev->item.type, 'd');
  CHECK_EQ_DOUBLE(list->prev->prev->item.data.d, 4.2, 0.001);

  list_free(list);
})

TEST_CASE(prepend_append, {
  node_t* const list = list_new();

  list_prepend(list, (item_t) { .type = 'i', .data.i = 42 });
  list_prepend(list, (item_t) { .type = 'i', .data.i = 69 });

  CHECK_EQ_INT(list->next->item.data.i, 69);
  CHECK_EQ_INT(list->next->next->item.data.i, 42);

  list_append(list, (item_t) { .type = 'i', .data.i = 21 });

  CHECK_EQ_INT(list->prev->item.data.i, 21);
  CHECK_EQ_INT(list->prev->prev->item.data.i, 42);
  CHECK_EQ_INT(list->next->next->next->item.data.i, 21);

  list_free(list);
})

TEST_CASE(delete, {
  node_t* const list = list_new();

  item_t first = { .type = 'i', .data.i = 69 };
  item_t second = { .type = 'i', .data.i = 42 };
  item_t third = { .type = 'i', .data.i = 21 };
  list_append(list, first);
  list_append(list, second);
  list_append(list, third);

  item_t result;
  result = list_elem_remove(list->next->next);
  CHECK_TRUE(item_equal(second, result));

  result = list_elem_remove(list->prev->prev);
  CHECK_TRUE(item_equal(first, result));

  result = list_elem_remove(list->next);
  CHECK_TRUE(item_equal(third, result));

  // Ensure we do not delete the sentinel and instead return ITEM_NULL.
  result = list_elem_remove(list->prev);
  CHECK_TRUE(item_equal(ITEM_NULL, result));

  list_free(list);
})

TEST_CASE(length, {
  node_t* const list = list_new();

  CHECK_TRUE(list_empty(list) != 0);
  CHECK_EQ_INT(list_length(list), 0);

  list_prepend(list, (item_t) { .type = 'i', .data.i = 42 });
  CHECK_EQ_INT(list_length(list), 1);
  list_prepend(list, (item_t) { .type = 'i', .data.i = 69 });
  CHECK_EQ_INT(list_length(list), 2);
  list_append(list, (item_t) { .type = 'i', .data.i = 21 });
  CHECK_EQ_INT(list_length(list), 3);

  (void) list_elem_remove(list->next);
  CHECK_EQ_INT(list_length(list), 2);
  (void) list_elem_remove(list->next);
  CHECK_EQ_INT(list_length(list), 1);
  (void) list_elem_remove(list->next);
  CHECK_EQ_INT(list_length(list), 0);

  list_free(list);
})

TEST_CASE(indexing, {
  printf("--- Begin indexing_at\n");
  node_t* const list = list_new();
  list_append(list, (item_t) { .type = 'i', .data.i = 69 });
  list_append(list, (item_t) { .type = 'i', .data.i = 42 });
  list_append(list, (item_t) { .type = 'i', .data.i = 21 });

  printf("Real values:\n");
  printf("list %d: %i%c\n", 0, list->next->item.data.i, list->next->item.type);
  printf("list %d: %i%c\n", 1, list->next->next->item.data.i, list->next->next->item.type);
  printf("list %d: %i%c\n", 2, list->next->next->next->item.data.i, list->next->next->next->item.type);

  printf("Indexed values:\n");
  for (int i = -3; i <= 2; i++) {
    node_t* pos = list_at(list, i);
    printf("list[%d]: %i%c\n", i, pos->item.data.i, pos->item.type);
  }

  // Indexing by digit.
  CHECK_TRUE(item_equal(list_at(list, 0)->item, (item_t) { .type = 'i', .data.i = 69 }));
  CHECK_TRUE(item_equal(list_at(list, 1)->item, (item_t) { .type = 'i', .data.i = 42 }));
  CHECK_TRUE(item_equal(list_at(list, 2)->item, (item_t) { .type = 'i', .data.i = 21 }));

  // Reverse indexing
  CHECK_TRUE(item_equal(list_at(list, -1)->item, (item_t) { .type = 'i', .data.i = 21 }));
  CHECK_TRUE(item_equal(list_at(list, -2)->item, (item_t) { .type = 'i', .data.i = 42 }));
  CHECK_TRUE(item_equal(list_at(list, -3)->item, (item_t) { .type = 'i', .data.i = 69 }));

  // Out of bounds.
  CHECK_TRUE(list_at(list, 3) == NULL);
  CHECK_TRUE(list_at(list, -4) == NULL);
  // Large values should not affect the speed of return.
  CHECK_TRUE(list_at(list, 100000000) == NULL);
  CHECK_TRUE(list_at(list, -100000000) == NULL);

  list_free(list);
  printf("--- Finish indexing_at\n");
})

TEST_CASE(find, {
  node_t* const list = list_new();

  item_t first = { .type = 'i', .data.i = 69 };
  item_t second = { .type = 'i', .data.i = 42 };
  item_t third = { .type = 'i', .data.i = 21 };
  list_append(list, first);
  list_append(list, second);
  list_append(list, third);

  node_t* result;
  result = list_find(list, third);
  CHECK_TRUE(item_equal(result->item, third));
  result = list_find(list, first);
  CHECK_TRUE(item_equal(result->item, first));
  result = list_find(list, second);
  CHECK_TRUE(item_equal(result->item, second));

  list_free(list);
})

MAIN_RUN_TESTS(create_and_free, insert, prepend_append, delete, length, indexing, find)
