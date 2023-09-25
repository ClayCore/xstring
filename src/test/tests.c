#include <assert.h>
#include <stdio.h>
#include <string.h>


#include "test/tests.h"
#include "util/aliases.h"
#include "xstring.h"

/******************************************************************************
 * TESTS BEGIN
 *****************************************************************************/

/******************************************************************************
 * @test test_empty
 *
 * Should always be valid, unless the macro is broken
 *****************************************************************************/
TEST_BEGIN(empty)
TEST_END(empty)

/******************************************************************************
 * @test test_xstring_new
 *
 * Allocates a new xstring using `xstring_new`. Valid when the allocation
 * is succesful, the buffer is NULL and length is zero, to indicate
 * the newly allocated string is empty.
 *****************************************************************************/
TEST_BEGIN(xstring_new)

xstring *str = NULL;

str = xstring_new();

if (str == NULL)
        result.success = false;
else if (str->len != 0)
        result.success = false;
else if (str->ptr != NULL)
        result.success = false;

xstring_free(str);

TEST_END(xstring_new)

/******************************************************************************
 * @test test_xstring_from
 *
 * Instantiates a new xstring using a predefined c-style string.
 * Valid when the allocation is succesful, the buffer is identical to
 * the source c-style string and the length matches.
 *****************************************************************************/
TEST_BEGIN(xstring_from)

char const buf[] = "test_from";
usize len        = sizeof(buf) / sizeof(*buf);

xstring *str = xstring_from(buf, len);
if (str == NULL)
        result.success = false;
else if (str->ptr == NULL)
        result.success = false;
else if (str->len != len)
        result.success = false;
else if (strncmp(buf, str->ptr, len) != XSTRING_EQUAL)
        result.success = false;

xstring_free(str);

TEST_END(xstring_from)

/******************************************************************************
 * @test test_xstring_copy
 *
 * Performs a deep copy of a source string.
 * Valid when the objects are identical.
 *****************************************************************************/
TEST_BEGIN(xstring_copy)

char const buf[] = "test_copy";
usize len        = sizeof(buf) / sizeof(*buf);

xstring *src  = xstring_from(buf, len);
xstring *copy = xstring_copy(src);

if (!xstring_eq(src, copy))
        result.success = false;
else if (strncmp(src->ptr, copy->ptr, len) != XSTRING_EQUAL)
        result.success = false;
else if (src->len != copy->len)
        result.success = false;

xstring_free(src);
xstring_free(copy);

TEST_END(xstring_copy)

/******************************************************************************
 * @test test_xstring_init
 *
 * Initializes an empty xstring object.
 * Valid when the object, buffers and their lengths used to create it
 * are identical.
 *****************************************************************************/
TEST_BEGIN(xstring_init)

char const buf[] = "test_init";
usize len        = sizeof(buf) / sizeof(*buf);

xstring *str = xstring_new();
xstring_init(str, buf, len);

if (str->len != len)
        result.success = false;
else if (strncmp(str->ptr, buf, len) != XSTRING_EQUAL)
        result.success = false;

xstring_free(str);

TEST_END(xstring_init)

/******************************************************************************
 * @test test_xstring_free
 *
 * Creates a new string and immediately deallocates it using the provided
 * free method.
 * Valid when the allocation and reclamation are succesful.
 *****************************************************************************/
TEST_BEGIN(xstring_free)

xstring *str1 = NULL;
xstring *str2 = NULL;

str1 = xstring_new();
if (!str1)
        result.success = false;

xstring_free(str1);
if (str1)
        result.success = false;

str2 = xstring_from("test_free", 9);
if (!str2)
        result.success = false;

xstring_free(str2);
if (str2)
        result.success = false;

TEST_END(xstring_free)

/******************************************************************************
 * TESTS END
 *****************************************************************************/

/******************************************************************************
 * Prettifies a provided test result
 *
 * @param[in] res test result
 * @param[in] idx index of the test
 *****************************************************************************/
static void test_display(test_result_t res, usize idx)
{
        char const *format = "(TEST %zu):\n\tname: [%s]\n\tresult: [%s]\n";
        fprintf(stdout, format, idx, res.name, res.success ? "true" : "false");
}

/******************************************************************************
 * Runs all the defined tests.
 *****************************************************************************/
void xstring_run_tests(void)
{
        usize iter = 0, num_tests = 0;

        // clang-format off
        test_result_t static (*tests[])(void) = {
                test_empty,
                test_xstring_new,
                test_xstring_init,
                test_xstring_from,
                test_xstring_copy,
                test_xstring_free,
        };
        // clang-format on

        num_tests = sizeof(tests) / sizeof(*tests);
        for (iter = 0; iter < num_tests; ++iter) {
                test_result_t test = tests[iter]();
                test_display(test, iter);
        }
}
