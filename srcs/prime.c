#include "prime.h"

size_t is_prime(const size_t n) {
    if (n <= 1) return 0;
    else if (n == 2 || n == 3) return 1;
    if (n % 2 == 0 || n % 3 == 0) return 0;

    size_t i = 5;
    while (i * i <= n) {
        if (n % i == 0 || n % (i + 2) == 0) return 0;
        i += 6;
    }

    return 1;
}

size_t next_prime(size_t n) {
    while (!is_prime(n)) n++;
    return n;
}