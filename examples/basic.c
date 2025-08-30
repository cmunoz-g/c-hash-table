#include <stdio.h>
#include "hash_table.h"

int main() {
    ht *h = ht_create();
    ht_set(h, "luka", "doncic");
    printf("%s\n", ht_get(h, "luka"));
    ht_remove(h, "luka");
    printf("%s\n", ht_get(h, "luka") ? "found" : "not found");
    ht_destroy(h);
    return 0;
}