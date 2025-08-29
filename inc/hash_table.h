#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

#define HT_PRIME_1 131
#define HT_PRIME_2 257


typedef struct {
    char *key;
    char *value;
} ht_item;

typedef struct {
    ht_item **items;
    size_t size;
    size_t count;
} ht;

static ht_item HT_DELETED_ITEM = {NULL, NULL}; // in order to preserve the collision chain, we cannot remove an item from the table. Instead, it is set to point to a sentinel item

#endif