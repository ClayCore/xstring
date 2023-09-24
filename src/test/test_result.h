#ifndef XSTRING_TEST_RESULT_GUARD_
#define XSTRING_TEST_RESULT_GUARD_

#include <stdbool.h>

typedef struct
{
        char const *name;
        bool success;
} test_result_t;

#endif /* XSTRING_TEST_RESULT_GUARD_ */
