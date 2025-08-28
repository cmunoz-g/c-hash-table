#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct {
    char *key;
    char *value;
} ht_item;

typedef struct {
    ht_item **items;
    size_t size;
    size_t count;
} ht;

#endif