#pragma once
#include <stdint.h>
#include <stdbool.h>
#define false 0
#define true 1
#define FALSE 0
#define TRUE 1

typedef uint8_t u8;
typedef int8_t s8;
typedef uint16_t u16;
typedef int16_t s16;
typedef uint32_t u32;
typedef int32_t s32;
typedef uint64_t u64;
typedef int64_t s64;
// TODO: find a way to do this portable
typedef float f32;
typedef double f64;

#define UREG_MAX INTPTR_MAX
#if INTPTR_MAX == INT32_MAX
typedef uint32_t ureg;  // equivalent to size_t
typedef int32_t sreg;
typedef uint16_t uregh;
typedef int16_t sregh;
typedef uint8_t uregq;
typedef int8_t sregq;
#elif INTPTR_MAX == INT64_MAX
typedef uint64_t ureg;  // equivalent to size_t
typedef int64_t sreg;
typedef uint32_t uregh;
typedef int32_t sregh;
typedef uint16_t uregq;
typedef int16_t sregq;
#else
#error "Environment not 32 or 64-bit."
#endif
