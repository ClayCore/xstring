#ifndef XSTRING_TESTS_GUARD_
#define XSTRING_TESTS_GUARD_

#include "test/test_result.h"

#ifndef TEST_BEGIN
#define TEST_BEGIN(n)                   \
        test_result_t test_##n(void)    \
        {                               \
                test_result_t result;   \
                result.success = false; \
                result.name    = __func__;
#endif /* TEST_END */

#ifndef TEST_END
#define TEST_END(n)            \
        result.success = true; \
        return (result);       \
        }
#endif /* TEST_END */

void xstring_run_tests(void);

#endif /* XSTRING_TESTS_GUARD_ */
