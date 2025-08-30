#ifndef PRIME_H
#define PRIME_H

#include <stdlib.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

bool is_prime(size_t n);
size_t next_prime(size_t n);

#ifdef __cplusplus
}
#endif

#endif /* PRIME_H */