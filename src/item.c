// Copyright © 2024 soupglasses <sofi+git@mailbox.org>
//
// Licensed under the EUPL, with extension of article 5 (compatibility
// clause) to any licence for distributing derivative works that have
// been produced by the normal use of the Work as a library.

#include <stdlib.h>
#include <string.h>
#include "item.h"

// A non zero return value means its equal.
int item_equal(item_t left, item_t right) {
  if (left.type == right.type || ((left.type == 's' || left.type == 'S') && (right.type == 's' || right.type == 'S')))
    switch (left.type) {
      case 'i':
        return (left.data.i == right.data.i);
      case 'd':
        return (left.data.d == right.data.d);
      case 'c':
        return (left.data.c == right.data.c);
      case 's':
      case 'S':
        // Both `s` and `S` are defined as `char *`, so this is okay for lookup of either.
        return (strcmp(left.data.s, right.data.s) == 0);
      case 'p':
        return (left.data.p == right.data.p);
    };
  return 0;
}

void item_free(item_t item) {
  switch (item.type) {
    case 's':
      free(item.data.s);
    case 'p':
      free(item.data.p);
  };
}
