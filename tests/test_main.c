#include "test.h"

int g_test_failures = 0;

int main() {

    if (g_test_failures) {
        fprintf(stderr, "# of failed tests: %d\n", g_test_failures);
        return EXIT_FAILURE;
    }

    printf("All tests passed\n");
    return EXIT_SUCCESS;
}