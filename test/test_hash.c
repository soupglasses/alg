// Copyright © 2024 soupglasses <sofi+git@mailbox.org>
//
// Licensed under the EUPL, with extension of article 5 (compatibility
// clause) to any licence for distributing derivative works that have
// been produced by the normal use of the Work as a library.

#include <string.h>
#include "mtest.h"
#include "hash.h"

TEST_CASE(djb2_sanity, {
  // Manually generated hash outside our implementation.
  CHECK_TRUE(djb2_hash("Germany") == 229425627781240ul);
})

TEST_CASE(creation, {
  hash_t* hash = hash_new(1);
  CHECK_EQ_INT(hash->size, 1);
  hash_free(hash);
})

TEST_CASE(indexing, {
  hash_t* hash = hash_new(12);
  unsigned long germany_djb2 = 229425627781240ul;

  hash->keys[germany_djb2 % 12] = germany_djb2;
  CHECK_EQ_INT(hash_index(hash, "Germany"), (int) (germany_djb2 % 12));

  hash_free(hash);
})

TEST_CASE(linear_probing, {
  hash_t* hash = hash_new(12);
  unsigned long germany_djb2 = 229425627781240ul;

  hash->keys[(germany_djb2 % 12)] = 1234567890ul;
  hash->keys[(germany_djb2 % 12) + 1] = germany_djb2;

  CHECK_EQ_INT(hash_index(hash, "Germany"), (int) (germany_djb2 % 12) + 1);

  hash_free(hash);
})

TEST_CASE(delete, {
  hash_t* hash = hash_new(12);
  unsigned long germany_djb2 = djb2_hash("Germany");

  hash->keys[(germany_djb2 % 12)] = germany_djb2;
  CHECK_EQ_INT(hash_index(hash, "Germany"), (int) (germany_djb2 % 12));

  hash_remove(hash, "Germany");
  // No more Germany keys shall exist.
  CHECK_EQ_INT(hash_index(hash, "Germany"), -1);

  hash_free(hash);
})

TEST_CASE(delete_linear_probing, {
  hash_t* hash = hash_new(12);
  unsigned long germany_djb2 = 229425627781240ul;

  // We have to fake it, because we are using identical keys.
  hash->keys[(germany_djb2 % 12)] = germany_djb2;
  hash->keys[(germany_djb2 % 12) + 1] = germany_djb2;
  hash->keys[(germany_djb2 % 12) + 2] = germany_djb2;

  hash_remove(hash, "Germany");
  CHECK_TRUE(hash->keys[(germany_djb2 % 12) + 2] == 0);
  CHECK_TRUE(hash->keys[(germany_djb2 % 12) + 1] == germany_djb2);
  CHECK_EQ_INT(hash_index(hash, "Germany"), (int) (germany_djb2 % 12));

  hash_remove(hash, "Germany");
  // Ensure our `+ 1` germany djb2 gets moved back to `+ 0`.
  CHECK_TRUE(hash->keys[(germany_djb2 % 12) + 1] == 0);
  CHECK_EQ_INT(hash_index(hash, "Germany"), (int) (germany_djb2 % 12));

  hash_remove(hash, "Germany");
  // No more Germany keys shall exist.
  CHECK_EQ_INT(hash_index(hash, "Germany"), -1);

  hash_free(hash);
})

TEST_CASE(smoke, {
  hash_t* hash = hash_new(12);
  REQUIRE_EQ_INT(hash_count(hash), 0);

  int i = 89;
  hash_put(hash, "Germany", &i);
  REQUIRE_EQ_INT(hash_count(hash), 1);

  int* value = hash_get(hash, "Germany");
  REQUIRE_EQ_INT(*value, 89);

  hash_free(hash);
})

MAIN_RUN_TESTS(djb2_sanity,
               creation,
               indexing,
               linear_probing,
               delete,
               delete_linear_probing,
               smoke)
