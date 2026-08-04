#include "../headers/havok/hkRadixSort.h"

/* hkRadixSort::sort32 @ 0x83FAC960
   Stable least-significant-digit radix sort of `numData` SortData32 entries by
   their 32-bit key, using `scratch` (same length) as ping-pong storage. Four
   byte-wide passes leave the sorted result back in `data`.

   DEVIATION: the binary builds all four 256-bucket histograms in a single fused
   pass and then skips any pass whose key byte is constant (doing a final buffer
   copy when an odd number of passes ran). That micro-optimization is
   reconstructed here as four unconditional passes; the ordering is identical and
   the result still lands in `data` (even pass count). */
void hkRadixSort_sort32(hkRadixSort_SortData32 *data, int numData, hkRadixSort_SortData32 *scratch)
{
    int histogram[4][256];
    int i, b;
    hkRadixSort_SortData32 *src = data;
    hkRadixSort_SortData32 *dst = scratch;

    for (b = 0; b < 4; ++b)
        for (i = 0; i < 256; ++i)
            histogram[b][i] = 0;

    /* Single fused counting pass over all four key bytes. */
    for (i = 0; i < numData; ++i)
    {
        unsigned int key = data[i].m_key;
        ++histogram[0][key & 0xFF];
        ++histogram[1][(key >> 8) & 0xFF];
        ++histogram[2][(key >> 16) & 0xFF];
        ++histogram[3][(key >> 24) & 0xFF];
    }

    /* Convert each histogram to a starting-offset table (exclusive prefix sum). */
    for (b = 0; b < 4; ++b)
    {
        int sum = 0;
        for (i = 0; i < 256; ++i)
        {
            int c = histogram[b][i];
            histogram[b][i] = sum;
            sum += c;
        }
    }

    /* Four stable scatter passes, ping-ponging src/dst. */
    for (b = 0; b < 4; ++b)
    {
        int shift = b * 8;
        for (i = 0; i < numData; ++i)
        {
            unsigned int bucket = (src[i].m_key >> shift) & 0xFF;
            dst[histogram[b][bucket]++] = src[i];
        }
        {
            hkRadixSort_SortData32 *tmp = src;
            src = dst;
            dst = tmp;
        }
    }
    /* After four passes src == data again. */
}
