#ifndef DATUM_INDEX_H
#define DATUM_INDEX_H

/* Blam datum-index (handle) packing — the canonical cseries idiom, reconstructed here on the
 * same basis as DATA_ARRAY_ELEMENT / BIT_VECTOR_* (a value-preserving macro over an idiom the
 * decompiler spelled as raw shift/mask arithmetic). A datum index is a 32-bit handle whose HIGH
 * word is a monotonically-incrementing salt (the "identifier", detects stale handles) and whose
 * LOW word is the absolute slot in the data array:
 *
 *     31                    16 15                     0
 *    +------------------------+------------------------+
 *    |   identifier (salt)    |    absolute index      |
 *    +------------------------+------------------------+
 *
 * These macros do NOT change any bit pattern — `DATUM_INDEX_TO_ABSOLUTE_INDEX(x)` is exactly
 * `x & 0xFFFF`, etc. NOTE the locked project convention: a raw sentinel COMPARE (`index == -1`,
 * `(short)index == 0xFFFF`) stays raw and is NOT rewritten to any macro — only the pack/unpack
 * ARITHMETIC is expressed through these. */

#define DATUM_INDEX_TO_ABSOLUTE_INDEX(datum_index) ((datum_index) & 0xFFFF)
#define DATUM_INDEX_TO_IDENTIFIER(datum_index)     ((unsigned int)(datum_index) >> 16)
#define BUILD_DATUM_INDEX(identifier, absolute_index) \
    (((unsigned int)(unsigned short)(identifier) << 16) | (unsigned short)(absolute_index))

#endif /* DATUM_INDEX_H */
