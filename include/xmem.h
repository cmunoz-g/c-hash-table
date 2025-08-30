#ifndef XMEM_H
#define XMEM_H

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

void *xmalloc(size_t size);
void *xcalloc(size_t nmemb, size_t size);
char *xstrdup(const char *s);

#ifdef __cplusplus
}
#endif

#endif /* XMEM_H */