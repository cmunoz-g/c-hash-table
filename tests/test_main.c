#include "test.h"

int g_test_failures = 0;

void test_ht_crud();
void test_ht_overwrite();
void test_ht_collisions();
void test_ht_resize();
void test_ht_edge_cases();

int main() {

    
    test_ht_crud();
    test_ht_overwrite();
    test_ht_collisions();
    test_ht_resize();
    test_ht_edge_cases();

    if (g_test_failures) {
        fprintf(stderr, "# of failed tests: %d\n", g_test_failures);
        return EXIT_FAILURE;
    }

    printf("All tests passed\n");
    return EXIT_SUCCESS;
}