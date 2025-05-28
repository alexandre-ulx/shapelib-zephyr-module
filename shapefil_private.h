#ifndef SHAPEFILE_PRIVATE_ZEPHYR_H_INCLUDED
#define SHAPEFILE_PRIVATE_ZEPHYR_H_INCLUDED

#include "shapefil.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <zephyr/sys/byteorder.h>

#ifdef __cplusplus
#define STATIC_CAST(type, x) static_cast<type>(x)
#define REINTERPRET_CAST(type, x) reinterpret_cast<type>(x)
#define CONST_CAST(type, x) const_cast<type>(x)
#define SHPLIB_NULLPTR nullptr
#else
#define STATIC_CAST(type, x) ((type)(x))
#define REINTERPRET_CAST(type, x) ((type)(x))
#define CONST_CAST(type, x) ((type)(x))
#define SHPLIB_NULLPTR NULL
#endif

// Define endianess for Zephyr (STM32H7 is little-endian by default)
#ifdef CONFIG_BIG_ENDIAN
#define SHP_BIG_ENDIAN 1
#endif

/************************************************************************/
/*        Little endian <==> big endian byte swap macros.               */
/************************************************************************/

#define _SHP_SWAP32(x) (sys_swap32(STATIC_CAST(uint32_t, x)))
#define _SHP_SWAP64(x) (sys_swap64(STATIC_CAST(uint64_t, x)))

// In-place uint32_t* swap
#define SHP_SWAP32(p) \
    *REINTERPRET_CAST(uint32_t *, p) = _SHP_SWAP32(*REINTERPRET_CAST(uint32_t *, p))

// In-place uint64_t* swap
#define SHP_SWAP64(p) \
    *REINTERPRET_CAST(uint64_t *, p) = _SHP_SWAP64(*REINTERPRET_CAST(uint64_t *, p))

// In-place double* swap (manual as Zephyr has no direct swap for double)
#define SHP_SWAPDOUBLE(x) \
    do { \
        uint64_t _n64; \
        memcpy(&_n64, (x), 8); \
        _n64 = sys_swap64(_n64); \
        memcpy((x), &_n64, 8); \
    } while (0)

// Copy double* swap
#define SHP_SWAPDOUBLE_CPY(dst, src) \
    do { \
        uint64_t _n64; \
        memcpy(&_n64, (src), 8); \
        _n64 = sys_swap64(_n64); \
        memcpy((dst), &_n64, 8); \
    } while (0)

#endif // SHAPEFILE_PRIVATE_ZEPHYR_H_INCLUDED