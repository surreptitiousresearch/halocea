#pragma once

/* hkRadixSort::SortData32 — a 32-bit sort key paired with a caller payload word.
   DB (types_members): offset 0 is a 4-byte union (the key), offset 4 m_userData.
   The union arm used by the sweep is the plain 32-bit key, modelled here as
   m_key so the ordering read stays legible. */
typedef struct hkRadixSort_SortData32
{
    unsigned int m_key;      /* 0x00 (DB: anonymous union ___u0) */
    unsigned int m_userData; /* 0x04 */
} hkRadixSort_SortData32;

/* hkRadixSort::sort32 — stable LSD radix sort of `numData` SortData32 entries by
   m_key, in place. `scratch` is a caller-provided temporary of the same length.
   Extern boundary — not reversed here. */
extern void hkRadixSort_sort32(hkRadixSort_SortData32 *data, int numData,
                               hkRadixSort_SortData32 *scratch);
