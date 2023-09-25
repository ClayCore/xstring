#ifndef XSTRING_DEFS_GUARD_
#define XSTRING_DEFS_GUARD_

#include "util/aliases.h"

struct xstring_s;
typedef struct xstring_s xstring;

struct xstring_list_s;
typedef struct xstring_list_s xstring_list;

struct xstring_s
{
        char *ptr;
        usize len;
};

struct xstring_list_s
{
        xstring *ptr;
        usize len;
};

#endif /* XSTRING_DEFS_GUARD_ */
