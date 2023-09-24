#ifndef XSTRING_GUARD_
#define XSTRING_GUARD_

#include <stdbool.h>

#include "util/aliases.h"
#include "xstring_defs.h"


extern xstring *xstring_new(void);
extern xstring *xstring_from(char const *, usize);
extern xstring *xstring_copy(xstring *);
extern void xstring_init(xstring *, char const *, usize);

extern u64 xstring_raw_hash(char const *, usize);
extern u64 xstring_hash(xstring *);

extern bool xstring_eq(xstring *, xstring *);
extern bool xstring_eq_icase(xstring *, xstring *);

extern isize xstring_find(xstring *, char);
extern isize xstring_find_rev(xstring *, char);

extern usize xstring_span(xstring *, xstring *);
extern usize xstring_span_rev(xstring *, xstring *);

extern void xstring_to_lower(xstring *);
extern void xstring_to_upper(xstring *);

extern void *xstring_filter(xstring *, xstring *);

extern xstring_list *xstring_split(xstring *, xstring *);

extern void xstring_free(xstring *);


extern xstring_list *xstring_list_new(void);

extern void xstring_list_push(xstring_list *, xstring *);
extern xstring *xstring_list_pop(xstring_list *);

extern void xstring_list_free(xstring_list *);

#endif /* XSTRING_GUARD_ */
