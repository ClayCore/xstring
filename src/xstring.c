#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "util/aliases.h"
#include "xstring.h"
#include "xstring_defs.h"

/******************************************************************************
 * STATICS AND CONSTANTS BEGIN
 *****************************************************************************/

/******************************************************************************
 * Look-up table for fast 'to_lower' transformation
 *****************************************************************************/
char const static XSTR_LOWER_LUT[] = {
        [' '] = ' ', ['!'] = '!', ['\"'] = '\"', ['#'] = '#',   ['$'] = '$',
        ['%'] = '%', ['&'] = '&', ['\''] = '\'', ['*'] = '*',   ['+'] = '+',
        [','] = ',', ['-'] = '-', ['='] = '=',   ['`'] = '`',   ['^'] = '^',
        ['~'] = '~', ['.'] = '.', ['/'] = '/',   ['\\'] = '\\', ['|'] = '|',
        [':'] = ':', [';'] = ';', ['('] = '(',   [')'] = ')',   ['{'] = '{',
        ['}'] = '}', ['<'] = '<', ['>'] = '>',   ['['] = '[',   [']'] = ']',
        ['_'] = '_', ['0'] = '0', ['1'] = '1',   ['2'] = '2',   ['3'] = '3',
        ['4'] = '4', ['5'] = '5', ['6'] = '6',   ['7'] = '7',   ['8'] = '8',
        ['9'] = '9', ['a'] = 'a', ['A'] = 'a',   ['b'] = 'b',   ['B'] = 'b',
        ['c'] = 'c', ['C'] = 'c', ['d'] = 'd',   ['D'] = 'd',   ['e'] = 'e',
        ['E'] = 'e', ['f'] = 'f', ['F'] = 'f',   ['g'] = 'g',   ['G'] = 'g',
        ['h'] = 'h', ['H'] = 'h', ['i'] = 'i',   ['I'] = 'i',   ['j'] = 'j',
        ['J'] = 'j', ['k'] = 'k', ['K'] = 'k',   ['l'] = 'l',   ['L'] = 'l',
        ['m'] = 'm', ['M'] = 'm', ['n'] = 'n',   ['N'] = 'n',   ['o'] = 'o',
        ['O'] = 'o', ['p'] = 'p', ['P'] = 'p',   ['q'] = 'q',   ['Q'] = 'q',
        ['r'] = 'r', ['R'] = 'r', ['s'] = 's',   ['S'] = 's',   ['t'] = 't',
        ['T'] = 't', ['u'] = 'u', ['U'] = 'u',   ['v'] = 'v',   ['V'] = 'v',
        ['w'] = 'w', ['W'] = 'w', ['x'] = 'x',   ['X'] = 'x',   ['y'] = 'y',
        ['Y'] = 'y', ['z'] = 'z', ['Z'] = 'z',   [1] = 1,       [2] = 2,
        [3] = 3,     [4] = 4,     [5] = 5,       [6] = 6,       [7] = 7,
        [8] = 8,     [9] = 9,     [10] = 10,     [11] = 11,     [12] = 12,
        [13] = 13,   [14] = 14,   [15] = 15,     [16] = 16,     [17] = 17,
        [18] = 18,   [19] = 19,   [20] = 20,     [21] = 21,     [22] = 22,
        [23] = 23,   [24] = 24,   [25] = 25,     [26] = 26,     [27] = 27,
        [28] = 28,   [29] = 29,   [30] = 30,     [31] = 31,     [127] = 127,
};
usize const static XSTR_LOWER_LUT_LEN = sizeof(XSTR_LOWER_LUT) / sizeof(*XSTR_LOWER_LUT);

/******************************************************************************
 * Look-up table for fast 'to_upper' transformation
 *****************************************************************************/
char const static XSTR_UPPER_LUT[] = {
        [' '] = ' ', ['!'] = '!', ['\"'] = '\"', ['#'] = '#',   ['$'] = '$',
        ['%'] = '%', ['&'] = '&', ['\''] = '\'', ['*'] = '*',   ['+'] = '+',
        [','] = ',', ['-'] = '-', ['='] = '=',   ['`'] = '`',   ['^'] = '^',
        ['~'] = '~', ['.'] = '.', ['/'] = '/',   ['\\'] = '\\', ['|'] = '|',
        [':'] = ':', [';'] = ';', ['('] = '(',   [')'] = ')',   ['{'] = '{',
        ['}'] = '}', ['<'] = '<', ['>'] = '>',   ['['] = '[',   [']'] = ']',
        ['_'] = '_', ['0'] = '0', ['1'] = '1',   ['2'] = '2',   ['3'] = '3',
        ['4'] = '4', ['5'] = '5', ['6'] = '6',   ['7'] = '7',   ['8'] = '8',
        ['9'] = '9', ['a'] = 'A', ['A'] = 'A',   ['b'] = 'B',   ['B'] = 'B',
        ['c'] = 'C', ['C'] = 'C', ['d'] = 'D',   ['D'] = 'D',   ['e'] = 'E',
        ['E'] = 'E', ['f'] = 'F', ['F'] = 'F',   ['g'] = 'G',   ['G'] = 'G',
        ['h'] = 'H', ['H'] = 'H', ['i'] = 'I',   ['I'] = 'I',   ['j'] = 'J',
        ['J'] = 'J', ['k'] = 'K', ['K'] = 'K',   ['l'] = 'L',   ['L'] = 'L',
        ['m'] = 'M', ['M'] = 'M', ['n'] = 'N',   ['N'] = 'N',   ['o'] = 'O',
        ['O'] = 'O', ['p'] = 'P', ['P'] = 'P',   ['q'] = 'Q',   ['Q'] = 'Q',
        ['r'] = 'R', ['R'] = 'R', ['s'] = 'S',   ['S'] = 'S',   ['t'] = 'T',
        ['T'] = 'T', ['u'] = 'U', ['U'] = 'U',   ['v'] = 'V',   ['V'] = 'V',
        ['w'] = 'W', ['W'] = 'W', ['x'] = 'X',   ['X'] = 'X',   ['y'] = 'Y',
        ['Y'] = 'Y', ['z'] = 'Z', ['Z'] = 'Z',   [1] = 1,       [2] = 2,
        [3] = 3,     [4] = 4,     [5] = 5,       [6] = 6,       [7] = 7,
        [8] = 8,     [9] = 9,     [10] = 10,     [11] = 11,     [12] = 12,
        [13] = 13,   [14] = 14,   [15] = 15,     [16] = 16,     [17] = 17,
        [18] = 18,   [19] = 19,   [20] = 20,     [21] = 21,     [22] = 22,
        [23] = 23,   [24] = 24,   [25] = 25,     [26] = 26,     [27] = 27,
        [28] = 28,   [29] = 29,   [30] = 30,     [31] = 31,     [127] = 127,
};
usize const static XSTR_UPPER_LUT_LEN = sizeof(XSTR_UPPER_LUT) / sizeof(*XSTR_UPPER_LUT);

/******************************************************************************
 * STATICS AND CONSTANTS END
 *****************************************************************************/

/******************************************************************************
 * UTILITY FUNCTIONS BEGIN
 *****************************************************************************/

/******************************************************************************
 * UTILITY FUNCTIONS END
 *****************************************************************************/

/******************************************************************************
 * IMPLEMENTATION BEGIN
 *****************************************************************************/

/******************************************************************************
 * Creates a new and empty xstring
 *
 * @return pointer to empty xstring
 *****************************************************************************/
xstring *xstring_new(void)
{
        xstring *str = NULL;

        str = malloc(sizeof *str);
        if (!str) {
                fprintf(stderr, "xstring_new: failed to alloc xstring\n");
                exit(EXIT_FAILURE);
        }

        return (str);
}

/******************************************************************************
 * Creates a new xstring from a c-style string
 *
 * @param[in] buf c-style string
 * @param[in] len length of c-style string
 * @return pointer to new xstring
 *****************************************************************************/
xstring *xstring_from(char const *buf, usize len)
{
        xstring *str = NULL;
        usize length = len;

        if (buf && len == 0) {
                char const *cpy = buf;
                while (*cpy++) {
                        length++;
                }

                assert(length > 0);
        }

        if (!buf) {
                fprintf(stderr, "xstring_from: buf is null\n");
                exit(EXIT_FAILURE);
        }

        str = malloc(sizeof *str);
        if (!str) {
                fprintf(stderr, "xstring_from: failed to alloc xstring\n");
                exit(EXIT_FAILURE);
        }

        str->len = length;
        str->ptr = calloc(str->len, sizeof *str->ptr);
        if (!str->ptr) {
                fprintf(stderr, "xstring_from: failed to alloc xstring buf\n");
                exit(EXIT_FAILURE);
        }

        memcpy(str->ptr, buf, str->len);

        return (str);
}

/******************************************************************************
 * Performs a heap copy of the source xstring
 *
 * @param[in] src source xstring
 * @return pointer to copied source
 *****************************************************************************/
xstring *xstring_copy(xstring *src)
{
        xstring *copy = NULL;

        if (!src) {
                fprintf(stderr, "xstring_copy: src is null\n");
                exit(EXIT_FAILURE);
        }

        if (!src->ptr || src->len == 0) {
                fprintf(stderr, "xstring_copy: src is empty\n");
                exit(EXIT_FAILURE);
        }

        copy = malloc(sizeof *copy);
        if (!copy) {
                fprintf(stderr, "xstring_copy: failed to alloc xstring\n");
                exit(EXIT_FAILURE);
        }

        copy->len = src->len;
        copy->ptr = calloc(copy->len, sizeof *copy->ptr);
        if (!copy->ptr) {
                fprintf(stderr, "xstring_copy: failed to alloc xstring buf\n");
                exit(EXIT_FAILURE);
        }

        memcpy(copy->ptr, src->ptr, copy->len);

        return (copy);
}

/******************************************************************************
 * Initialize an empty xstring with a c-style string
 *
 * @param[out] str xstring to init
 * @param[in] buf c-style string
 * @param[in] len length of c-style string
 * @return pointer to copied source
 *****************************************************************************/
void xstring_init(xstring *str, char const *buf, usize len)
{
        if (!str) {
                fprintf(stderr, "xstring_init: src is null\n");
                exit(EXIT_FAILURE);
        }

        if (str->ptr || str->len != 0) {
                fprintf(stderr, "xstring_init: src is not empty\n");
                exit(EXIT_FAILURE);
        }

        if (!buf || len == 0) {
                fprintf(stderr, "xstring_init: c-style string is empty\n");
                exit(EXIT_FAILURE);
        }

        str->len = len;
        str->ptr = calloc(str->len, sizeof *str->ptr);
        if (!str->ptr) {
                fprintf(stderr, "xstring_init: failed to alloc xstring buf\n");
                exit(EXIT_FAILURE);
        }

        memcpy(str->ptr, buf, str->len);
}

/******************************************************************************
 * Provides a hash for the c-style string
 *
 * @param[in] buf c-style string
 * @param[in] len length of c-style string
 * @return hash of the c-style string
 *****************************************************************************/
u64 xstring_raw_hash(char const *buf, usize len)
{
        u64 res   = (2 << 12) - 21;
        usize cnt = 0;

        if (!buf) {
                fprintf(stderr, "xstring_raw_hash: buf is null\n");
                exit(EXIT_FAILURE);
        }

        if (len == 0) {
                fprintf(stderr, "xstring_raw_hash: zero-length string\n");
                exit(EXIT_FAILURE);
        }

        while (cnt < len) {
                res = ((res << 5) + res) + buf[cnt];

                cnt++;
        }

        return (res);
}

/******************************************************************************
 * Provides a hash for the xstring
 *
 * @param[in] str xstring to hash
 * @return hash of provided xstring
 *****************************************************************************/
u64 xstring_hash(xstring *str)
{
        u64 res = 0;

        if (!str) {
                fprintf(stderr, "xstring_hash: str is null\n");
                exit(EXIT_FAILURE);
        }

        if (!str->ptr || str->len == 0) {
                fprintf(stderr, "xstring_hash: str is empty\n");
                exit(EXIT_FAILURE);
        }

        res = xstring_raw_hash(str->ptr, str->len);

        return (res);
}

/******************************************************************************
 * Case-sensitive comparison of two xstrings
 *
 * @param[in] lhs left operand
 * @param[in] rhs right operand
 * @return true if both xstrings are equal, false otherwise
 *****************************************************************************/
bool xstring_eq(xstring *lhs, xstring *rhs)
{
        u64 lhash = 0, rhash = 0;

        if (!lhs) {
                fprintf(stderr, "xstring_eq: lhs is null\n");
                exit(EXIT_FAILURE);
        }

        if (!lhs->ptr || lhs->len == 0) {
                fprintf(stderr, "xstring_eq: lhs is empty\n");
                exit(EXIT_FAILURE);
        }

        if (!rhs) {
                fprintf(stderr, "xstring_eq: rhs is null\n");
                exit(EXIT_FAILURE);
        }

        if (!rhs->ptr || rhs->len == 0) {
                fprintf(stderr, "xstring_eq: rhs is empty\n");
                exit(EXIT_FAILURE);
        }

        lhash = xstring_hash(lhs);
        rhash = xstring_hash(rhs);

        return (lhash == rhash);
}

/******************************************************************************
 * Case-insensitive comparison of two xstrings
 *
 * @param[in] lhs left operand
 * @param[in] rhs right operand
 * @return same as xstring_eq, except non case-sensitive
 *****************************************************************************/
bool xstring_eq_icase(xstring *lhs, xstring *rhs)
{
        /* TODO: rewrite to avoid copies and allocs */

        xstring *llhs = NULL, *rrhs = NULL;
        u64 lhash = 0, rhash = 0;
        bool result = false;

        if (!lhs) {
                fprintf(stderr, "xstring_eq: lhs is null\n");
                exit(EXIT_FAILURE);
        }

        if (!lhs->ptr || lhs->len == 0) {
                fprintf(stderr, "xstring_eq: lhs is empty\n");
                exit(EXIT_FAILURE);
        }

        if (!rhs) {
                fprintf(stderr, "xstring_eq: rhs is null\n");
                exit(EXIT_FAILURE);
        }

        if (!rhs->ptr || rhs->len == 0) {
                fprintf(stderr, "xstring_eq: rhs is empty\n");
                exit(EXIT_FAILURE);
        }

        llhs  = xstring_to_lower(lhs);
        lhash = xstring_hash(llhs);

        rrhs  = xstring_to_lower(rhs);
        rhash = xstring_hash(rrhs);

        result = (lhash == rhash);

        xstring_free(llhs);
        xstring_free(rrhs);

        return (result);
}

/******************************************************************************
 * Finds the first occurence of a character in xstring
 *
 * @param[in] str xstring to scan
 * @param[in] chr character to scan for
 * @return index of the character or -1 if not found
 *****************************************************************************/
isize xstring_find(xstring *str, char chr)
{
        bool found  = false;
        isize index = 0;
        usize iter  = 0;

        if (!str) {
                fprintf(stderr, "xstring_find: str is null\n");
                exit(EXIT_FAILURE);
        }

        if (!str->ptr || str->len == 0) {
                fprintf(stderr, "xstring_find: str is empty\n");
                exit(EXIT_FAILURE);
        }

        if (!chr || !isprint(chr)) {
                fprintf(stderr, "xstring_find: invalid character\n");
                exit(EXIT_FAILURE);
        }

        for (; iter < str->len; ++iter) {
                if (str->ptr[iter] == chr) {
                        found = true;
                        break;
                }
        }

        return (!found ? -1 : index);
}

/******************************************************************************
 * Finds the last occurence of a character in xstring
 *
 * @param[in] str xstring to scan
 * @param[in] chr character to scan for
 * @return index of the character or -1 if not found
 *****************************************************************************/
isize xstring_find_rev(xstring *str, char chr)
{
        bool found  = false;
        isize index = 0;
        usize iter  = 0;

        if (!str) {
                fprintf(stderr, "xstring_find_rev: str is null\n");
                exit(EXIT_FAILURE);
        }

        if (!str->ptr || str->len == 0) {
                fprintf(stderr, "xstring_find_rev: str is empty\n");
                exit(EXIT_FAILURE);
        }

        if (!chr || !isprint(chr)) {
                fprintf(stderr, "xstring_find_rev: invalid character\n");
                exit(EXIT_FAILURE);
        }

        iter = str->len;
        while (iter--) {
                if (str->ptr[iter] == chr) {
                        found = true;
                        break;
                }
        }

        return (!found ? -1 : index);
}

/******************************************************************************
 * Returns the run of a string consisting of a set of characters
 * in the filter.
 *
 * @param[in] str xstring to scan
 * @param[in] filter set of characters to scan for
 * @return length of the string until the filter is rejected
 *****************************************************************************/
usize xstring_span(xstring *str, xstring *filter)
{
        usize run = 0, iter = 0;
        isize idx = 0;

        if (!str) {
                fprintf(stderr, "xstring_span: str is null\n");
                exit(EXIT_FAILURE);
        }

        if (!str->ptr || str->len == 0) {
                fprintf(stderr, "xstring_span: str is empty\n");
                exit(EXIT_FAILURE);
        }

        if (!filter) {
                fprintf(stderr, "xstring_span: filter is null\n");
                exit(EXIT_FAILURE);
        }

        if (!filter->ptr || filter->len == 0) {
                fprintf(stderr, "xstring_span: filter is empty\n");
                exit(EXIT_FAILURE);
        }

        for (; iter < str->len; ++iter) {
                idx = xstring_find(filter, str->ptr[iter]);
                if (str->ptr[iter] == filter->ptr[idx]) {
                        run += 1;
                }
        }

        return (run);
}

/******************************************************************************
 * Returns the inverse of the run consisting of a set of characters
 * in the filter.
 *
 * @param[in] str xstring to scan
 * @param[in] filter set of characters to scan for
 * @return length of the string until the filter is accepted
 *****************************************************************************/
usize xstring_span_rev(xstring *str, xstring *filter)
{
        usize run = 0, iter = 0;
        isize idx = 0;

        if (!str) {
                fprintf(stderr, "xstring_span_rev: str is null\n");
                exit(EXIT_FAILURE);
        }

        if (!str->ptr || str->len == 0) {
                fprintf(stderr, "xstring_span_rev: str is empty\n");
                exit(EXIT_FAILURE);
        }

        if (!filter) {
                fprintf(stderr, "xstring_span_rev: filter is null\n");
                exit(EXIT_FAILURE);
        }

        if (!filter->ptr || filter->len == 0) {
                fprintf(stderr, "xstring_span_rev: filter is empty\n");
                exit(EXIT_FAILURE);
        }

        iter = str->len;
        while (iter--) {
                idx = xstring_find_rev(filter, str->ptr[iter]);
                if (str->ptr[iter] == filter->ptr[idx]) {
                        run += 1;
                }
        }

        return (run);
}

/******************************************************************************
 * Returns a copy of a string that is converted to lowercase
 *
 * @param[in] str xstring to convert
 * @return converted string
 *****************************************************************************/
xstring *xstring_to_lower(xstring *str)
{
        xstring *res = NULL;

        if (!str) {
                fprintf(stderr, "xstring_to_lower: str is null\n");
                exit(EXIT_FAILURE);
        }

        if (!str->ptr || str->len == 0) {
                fprintf(stderr, "xstring_to_lower: str is empty\n");
                exit(EXIT_FAILURE);
        }

        res = xstring_copy(str);
        xstring_to_lower_mut(res);

        return (res);
}

/******************************************************************************
 * Returns a copy of a string that is converted to uppercase
 *
 * @param[in] str xstring to convert
 * @return converted string
 *****************************************************************************/
xstring *xstring_to_upper(xstring *str)
{
        xstring *res = NULL;

        if (!str) {
                fprintf(stderr, "xstring_to_lower: str is null\n");
                exit(EXIT_FAILURE);
        }

        if (!str->ptr || str->len == 0) {
                fprintf(stderr, "xstring_to_lower: str is empty\n");
                exit(EXIT_FAILURE);
        }

        res = xstring_copy(str);
        xstring_to_upper_mut(res);

        return (res);
}

/******************************************************************************
 * Converts (in-place) all characters in the xstring to lowercase
 *
 * @param[in] str xstring to convert
 *****************************************************************************/
void xstring_to_lower_mut(xstring *str)
{
        usize iter = 0, idx = 0;
        char chr = 0;

        if (!str) {
                fprintf(stderr, "xstring_to_lower_mut: str is null\n");
                exit(EXIT_FAILURE);
        }

        if (!str->ptr || str->len == 0) {
                fprintf(stderr, "xstring_to_lower_mut: str is empty\n");
                exit(EXIT_FAILURE);
        }

        for (; iter < str->len; ++iter) {
                idx = (usize) (str->ptr[iter]);
                chr = XSTR_LOWER_LUT[idx];

                str->ptr[iter] = chr;
        }
}

/******************************************************************************
 * Converts (in-place) all characters in the xstring to uppercase
 *
 * @param[in] str xstring to convert
 *****************************************************************************/
void xstring_to_upper_mut(xstring *str)
{
        usize iter = 0, idx = 0;
        char chr = 0;

        if (!str) {
                fprintf(stderr, "xstring_to_lower_mut: str is null\n");
                exit(EXIT_FAILURE);
        }

        if (!str->ptr || str->len == 0) {
                fprintf(stderr, "xstring_to_lower_mut: str is empty\n");
                exit(EXIT_FAILURE);
        }

        for (; iter < str->len; ++iter) {
                idx = (usize) (str->ptr[iter]);
                chr = XSTR_UPPER_LUT[idx];

                str->ptr[iter] = chr;
        }
}

void *xstring_filter(xstring *, xstring *)
{
}

xstring_list *xstring_split(xstring *, xstring *)
{
}

/******************************************************************************
 * Frees the heap allocated xstring and its internal buffer
 *
 * @param[in] str xstring to free
 *****************************************************************************/
void xstring_free(xstring *str)
{
        if (str) {
                if (str->ptr) {
                        free(str->ptr);
                        str->ptr = NULL;
                }
                str->len = 0;

                free(str);
                str = NULL;
        }
}


xstring_list *xstring_list_new(void)
{
}

void xstring_list_push(xstring_list *, xstring *)
{
}
xstring *xstring_list_pop(xstring_list *)
{
}

void xstring_list_free(xstring_list *)
{
}

/******************************************************************************
 * IMPLEMENTATION END
 *****************************************************************************/
