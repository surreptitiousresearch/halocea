/* hkMergeOddEven @0x83FA0478 */
#include "../headers/havok/hkThreadMemory.h"

/* hkMergeOddEven — de-interleave step of the inverse wavelet lifting: given a
   buffer whose first n/2 entries are the "even" (approximation) samples and whose
   next n/2 entries are the "odd" (detail) samples, produce the interleaved signal
   even0, odd0, even1, odd1, ... back into dest.

   A scratch copy of the n source samples is taken from the per-thread stack
   allocator first (source and dest alias in the caller), then the two halves are
   woven together. The main loop is unrolled 4-wide with a remainder tail, exactly
   as the DB body; reproduced here as a straight interleave. */
void hkMergeOddEven(float *source, float *dest, int n)
{
    hkThreadMemory *tm = hkThreadMemory_getCurrent();
    int half = n >> 1;
    float *scratch;
    int i;

    /* Scratch buffer sized (n+4) floats rounded to 16 bytes, LIFO stack alloc. */
    scratch = (float *)hkThreadMemory_allocateStack(tm, (int)((4 * (n + 4)) & 0xFFFFFFF0));
    for (i = 0; i < n; ++i)
        scratch[i] = source[i];

    for (i = 0; i < half; ++i)
    {
        dest[2 * i]     = scratch[i];         /* even sample */
        dest[2 * i + 1] = scratch[half + i];  /* odd  sample */
    }

    hkThreadMemory_deallocateStack(tm, scratch);
}
