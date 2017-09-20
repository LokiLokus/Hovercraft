#pragma once
#include <stdint.h>
#include <stdbool.h>

typedef uint8_t u8;
typedef int8_t s8;
typedef uint16_t u16;
typedef int16_t s16;
typedef uint32_t u32;
typedef int32_t s32;
typedef uint64_t u64;
typedef int64_t s64;
typedef float f32;
typedef double f64;

#define SREG_MIN INTPTR_MIN
#define SREG_MAX INTPTR_MAX
#define UREG_MAX UINTPTR_MAX

#if INTPTR_MAX == INT32_MAX
typedef uint32_t ureg; // equivalent to size_t
typedef int32_t sreg; // equivalent to ssize_t
#elif INTPTR_MAX == INT64_MAX
typedef uint64_t ureg; // equivalent to size_t
typedef int64_t sreg; // equivalent to ssize_t
#else
#error "Environment neither 32 nor 64-bit."
#endif
