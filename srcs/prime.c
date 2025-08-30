#include "prime.h"

bool is_prime(size_t n) {
    if (n <= 1) return false;
    else if (n == 2 || n == 3) return true;
    if (n % 2 == 0 || n % 3 == 0) return false;

    size_t i = 5;
    while (i <= n / i) {
        if (n % i == 0 || n % (i + 2) == 0) return false;
        i += 6;
    }

    return true;
}

size_t next_prime(size_t n) {
    while (!is_prime(n)) n++;
    return n;
}