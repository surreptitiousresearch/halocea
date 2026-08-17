/* ?sortAabbs@hk1AxisSweep@@SAXPAUAabbInt@1@H@Z @0x83945B58 */
#include "../headers/havok/hk1AxisSweep.h"
#include "../headers/havok/hkRadixSort.h"
#include "../headers/havok/hkThreadMemory.h"

/* hk1AxisSweep::sortAabbs — radix-sort the AabbInt array ascending by m_min[0]
   (the sweep axis). A parallel hkRadixSort::SortData32 array carries {key, index}
   through the sort; the resulting permutation is then applied to the 32-byte
   AabbInt records.
 *
 * DEVIATION: the decompiler shows the per-thread bump/stack allocator inlined at
 * every scratch acquisition (top = word[8], limit = word[11], base-marker word[10],
 * vtable[20] = grow, vtable[24] = notify-empty on pop) and the record permutation
 * emitted as rotated, phase-shifted word copies. Both are expressed here in their
 * source-level forms: hkThreadMemory allocateStack/deallocateStack, and plain
 * indexed AabbInt copies. The scratch counts (padded to a multiple of 4, plus two
 * guard slots for the key buffers) reproduce the original byte sizes exactly. */
void hk1AxisSweep_sortAabbs(hk1AxisSweep_AabbInt *aabbs, int numAabbs)
{
    hkThreadMemory *tm = hkThreadMemory_getCurrent();
    int paddedCount = (numAabbs + 3) & ~3;
    int keyBytes = 8 * (paddedCount + 2);          /* (paddedCount+2) SortData32 */
    hkRadixSort_SortData32 *keys;
    hkRadixSort_SortData32 *scratch;
    hk1AxisSweep_AabbInt *sorted;
    int i;

    keys = (hkRadixSort_SortData32 *)hkThreadMemory_allocateStack(tm, keyBytes);
    for (i = 0; i < paddedCount; ++i)
    {
        keys[i].m_key = aabbs[i].m_min[0]; /* padding lanes read the 0xFFFFFFFF sentinels */
        keys[i].m_userData = (unsigned int)i;
    }

    scratch = (hkRadixSort_SortData32 *)hkThreadMemory_allocateStack(tm, keyBytes);
    hkRadixSort_sort32(keys, paddedCount, scratch);
    hkThreadMemory_deallocateStack(tm, scratch);

    /* Apply the sorted permutation via a temporary record buffer, then copy back. */
    sorted = (hk1AxisSweep_AabbInt *)hkThreadMemory_allocateStack(tm, 32 * numAabbs + 16);
    for (i = 0; i < numAabbs; ++i)
        sorted[i] = aabbs[keys[i].m_userData];
    for (i = 0; i < numAabbs; ++i)
        aabbs[i] = sorted[i];

    hkThreadMemory_deallocateStack(tm, sorted);
    hkThreadMemory_deallocateStack(tm, keys);
}
