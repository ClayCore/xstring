#ifndef XSTRING_ALIASES_GUARD_
#define XSTRING_ALIASES_GUARD_

#include <stddef.h>
#include <stdint.h>


typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef uintptr_t usize;
typedef intptr_t isize;

typedef usize uintptr;
typedef isize intptr;
typedef ptrdiff_t ptrdiff;

typedef float f32;
typedef double f64;
typedef long double f128;

/* for use in comparisons with stdlib strings */
typedef enum
{
        XSTRING_EQUAL = 0
} strequal;

#endif /* XSTRING_ALIASES_GUARD_ */
