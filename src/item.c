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
  switch (left.type) {
    case 'i':
      if (right.type != 'i') return 0;
      return (left.data.i == right.data.i);
    case 'd':
      if (right.type != 'd') return 0;
      return (left.data.d == right.data.d);
    case 'c':
      if (right.type != 'c') return 0;
      return (left.data.c == right.data.c);
    case 's':
    case 'S':
      if (right.type != 's' && right.type != 'S') return 0;
      // Both `s` and `S` are defined as `char *`, so this is okay for lookup.
      return (strcmp(left.data.s, right.data.s) == 0);
    case 'p':
      if (right.type != 'p') return 0;
      return (left.data.p == right.data.p);
  };
  // Since we use a char, its technically possible to not match above.
  // TODO: Handle this better, dont just say its not equal.
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
