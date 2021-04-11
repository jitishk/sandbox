#include <stddef.h>

typedef struct subtest_s {
        int i;
        char c;
        void *ptr;
        double d;
        char a[5];
} subtest_t;

typedef struct test_s {
        int i;
        subtest_t subtest;
        char c;
        void *ptr;
        double d;
        char a[5];

    } test_t ;

int main()
{

    test_t test;
    int offsetArray[4] = { offsetof(test_t, i ), offsetof( test_t, c ), offsetof(test_t, d ), offsetof(test_t, a )  } ;

    return 0 ;
}
