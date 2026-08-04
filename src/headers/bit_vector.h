#ifndef BIT_VECTOR_H
#define BIT_VECTOR_H

/* bit_vector — the Blam cseries 32-bit-word bit-vector idiom. The decompiler spells the
 * word/bit split as `v[(i) >> 5]` / `1 << ((i) & 0x1F)`; the original code used macros of
 * this family (cseries convention, same reconstruction basis as DATA_ARRAY_ELEMENT /
 * TAG_INSTANCE). Value-preserving: these expand to exactly the raw expressions. */

#define BIT_VECTOR_SIZE_IN_LONGS(bit_count) (((bit_count) + 31) >> 5)

#define BIT_VECTOR_TEST_FLAG(vector, index) \
    (((vector)[(index) >> 5] & (1 << ((index) & 31))) != 0)

#define BIT_VECTOR_SET_FLAG(vector, index) \
    ((vector)[(index) >> 5] |= (1 << ((index) & 31)))

#define BIT_VECTOR_CLEAR_FLAG(vector, index) \
    ((vector)[(index) >> 5] &= ~(1 << ((index) & 31)))

#endif /* BIT_VECTOR_H */
