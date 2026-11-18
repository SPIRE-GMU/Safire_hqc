#ifndef T1_LAYERS_ACCESS_H
#define T1_LAYERS_ACCESS_H
#include <stdint.h>
#include <assert.h>

// Host checks include the complete extents of every restricted array.
#ifdef T1_TEST_RESTRICT
#define T1_DISJOINT(a, na, b, nb) \
    assert((uintptr_t)(a) + sizeof(uint32_t) * (na) <= (uintptr_t)(b) || \
           (uintptr_t)(b) + sizeof(uint32_t) * (nb) <= (uintptr_t)(a))
#define T1_LAYER_ASSERT(condition) assert(condition)
#else
#define T1_DISJOINT(a, na, b, nb) ((void)0)
#define T1_LAYER_ASSERT(condition) ((void)0)
#endif
// Standard C++ attribute understood by host compilers and chess-clang.  This
// keeps all high layers in one tile on the same runtime-parameter layer body.
#define T1_LAYER_NOINLINE __attribute__((noinline))
typedef void (*t1_circuit)(uint32_t*, const uint32_t*);
#endif
