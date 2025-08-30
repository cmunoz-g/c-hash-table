#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "prime.h"

#define HT_PRIME_1 131
#define HT_PRIME_2 257
#define HT_DEFAULT_BASE_SIZE 53
#define HT_MAX_LOAD 70
#define HT_MIN_LOAD 10

typedef struct {
    char *key;
    char *value;
} ht_item;

typedef struct {
    ht_item **items;
    size_t base_size;
    size_t size;
    size_t count;
} ht;

#endif