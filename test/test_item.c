// Copyright © 2024 soupglasses <sofi+git@mailbox.org>
//
// Licensed under the EUPL, with extension of article 5 (compatibility
// clause) to any licence for distributing derivative works that have
// been produced by the normal use of the Work as a library.

#include <string.h>

#include "mtest.h"
#include "item.h"

TEST_CASE(strings, {
  char s_str[10] = "Test";
  char* d_str = malloc(strlen(s_str) + 1);
  strcpy(d_str, s_str);

  item_t static_string = { .type = 'S', .data.S = s_str };
  item_t dynamic_string = { .type = 's', .data.s = d_str };
  CHECK_TRUE(item_equal(static_string, static_string));
  CHECK_TRUE(item_equal(dynamic_string, dynamic_string));
  CHECK_TRUE(item_equal(static_string, dynamic_string));
  CHECK_TRUE(item_equal(dynamic_string, static_string));

  // Now lets retest and ensure these all fail for static.
  static_string = (item_t) { .type = 'S', .data.S = "tseT" };
  CHECK_FALSE(item_equal(static_string, dynamic_string));
  CHECK_FALSE(item_equal(dynamic_string, static_string));

  // Now lets retest and ensure these all fail for dynamic.
  static_string = (item_t) { .type = 'S', .data.S = "Test" };
  strcpy(d_str, "tesT");
  CHECK_FALSE(item_equal(static_string, dynamic_string));
  CHECK_FALSE(item_equal(dynamic_string, static_string));

  // Finally, lets reverse both.
  static_string = (item_t) { .type = 'S', .data.S = "tesT" };
  CHECK_TRUE(item_equal(static_string, dynamic_string));
  CHECK_TRUE(item_equal(dynamic_string, static_string));

  free(d_str);
})

MAIN_RUN_TESTS(strings)
