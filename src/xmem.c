#include "xmem.h"

void *xmalloc(size_t size) {
    void *p = malloc(size);
    if (!p) {
        fprintf(stderr, "malloc: OOM\n");
        exit(EXIT_FAILURE);
    }
    return p;
}

void *xcalloc(size_t nmemb, size_t size) {
    void *p = calloc(nmemb, size);
    if (!p) {
        fprintf(stderr, "calloc: OOM\n");
        exit(EXIT_FAILURE);
    }
    return p;
}

char *xstrdup(const char *s) {
    if (!s) return NULL;
    size_t len = strlen(s) + 1;
    char *p = xmalloc(len);
    memcpy(p, s, len);
    return p;
}