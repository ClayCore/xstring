#include <stdio.h>

#include "test/tests.h"
#include "util/aliases.h"

TEST_BEGIN(empty)
TEST_END(empty)

static void test_display(test_result_t res, usize idx)
{
        char const *format = "(TEST %zu):\n\tname: [%s]\n\tresult: [%s]\n";
        fprintf(stdout, format, idx, res.name, res.success ? "true" : "false");
}

void xstring_run_tests(void)
{
        usize iter = 0, num_tests = 0;

        test_result_t static (*tests[])(void) = { test_empty };

        num_tests = sizeof(tests) / sizeof(*tests);
        for (iter = 0; iter < num_tests; ++iter) {
                test_result_t test = tests[iter]();
                test_display(test, iter);
        }
}
