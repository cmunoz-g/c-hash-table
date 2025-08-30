#ifndef TEST_H
#define TEST_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

extern int g_test_failures;

#define ASSERT_TRUE(msg, cond) do { \
if (!(cond)) { fprintf(stderr, "Fail: %s (%s:%d)\n", msg, __FILE__, __LINE__); g_test_failures++; } \
} while (0)

#endif /* TEST_H */