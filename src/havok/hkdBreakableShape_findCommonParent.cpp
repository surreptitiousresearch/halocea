/* ?findCommonParent@hkdBreakableShape@@SAPBV1@PBV1@0@Z @0x8384ABD8 */
#include "../headers/havok/hkdBreakableShape.h"
#include "../headers/havok/hkInplaceArray.h"
#include "../headers/havok/hkThreadMemory.h"

extern void hkArrayUtil__reserveMore(void *array, int sizeElem); /* hkArrayUtil::_reserveMore */

/* hkdBreakableShape::findCommonParent — find the deepest breakable shape that is
   an ancestor of both `a` and `b` (or null if they share no ancestor). Builds the
   two ancestor chains (each starting with the shape itself) into scratch arrays,
   then compares them from the roots downward: the last agreeing entry is the
   nearest common parent.
   DEVIATION: the decompiler compared the two arrays with a raw pointer-difference
   trick (v11 = &B[last] - &A[last]); restored here to lockstep indices. */
const hkdBreakableShape *hkdBreakableShape_findCommonParent(const hkdBreakableShape *a,
                                                            const hkdBreakableShape *b)
{
    const hkdBreakableShape *aParent = a->m_parent;
    const hkdBreakableShape *bParent = b->m_parent;
    hkInplaceArray<const hkdBreakableShape *, 16> ancestorsA;
    hkInplaceArray<const hkdBreakableShape *, 16> ancestorsB;
    const hkdBreakableShape *commonParent;
    int ia, ib;

    if (aParent == bParent)
        return aParent;

    /* ancestorsA = [a, a->m_parent, a->m_parent->m_parent, ...] */
    ancestorsA.m_data = ancestorsA.m_storage;
    ancestorsA.m_storage[0] = a;
    ancestorsA.m_size = 1;
    ancestorsA.m_capacityAndFlags = 0x80000010; /* inline capacity 16, storage not owned */
    for (; aParent; aParent = aParent->m_parent)
    {
        if (ancestorsA.m_size == (ancestorsA.m_capacityAndFlags & 0x3FFFFFFF))
            hkArrayUtil__reserveMore(&ancestorsA.m_data, 4);
        ancestorsA.m_data[ancestorsA.m_size++] = aParent;
    }

    /* ancestorsB = [b, b->m_parent, b->m_parent->m_parent, ...] */
    ancestorsB.m_data = ancestorsB.m_storage;
    ancestorsB.m_size = 0;
    ancestorsB.m_capacityAndFlags = 0x80000010; /* inline capacity 16, storage not owned */
    if (ancestorsB.m_size == (ancestorsB.m_capacityAndFlags & 0x3FFFFFFF))
        hkArrayUtil__reserveMore(&ancestorsB.m_data, 4);
    ancestorsB.m_data[ancestorsB.m_size++] = b;
    for (; bParent; bParent = bParent->m_parent)
    {
        if (ancestorsB.m_size == (ancestorsB.m_capacityAndFlags & 0x3FFFFFFF))
            hkArrayUtil__reserveMore(&ancestorsB.m_data, 4);
        ancestorsB.m_data[ancestorsB.m_size++] = bParent;
    }

    /* Compare from the roots (last elements) downward while the chains agree; the
       last matching entry is the nearest common parent. */
    commonParent = nullptr;
    ia = ancestorsA.m_size - 1;
    ib = ancestorsB.m_size - 1;
    if (ancestorsA.m_data[ia] == ancestorsB.m_data[ib])
    {
        do
        {
            --ia;
            --ib;
        } while (ancestorsA.m_data[ia] == ancestorsB.m_data[ib]);
        commonParent = ancestorsA.m_data[ia + 1];
    }

    if (ancestorsB.m_capacityAndFlags >= 0)
        hkThreadMemory_deallocateChunk(hkThreadMemory_getCurrent(), ancestorsB.m_data,
                                       4 * ancestorsB.m_capacityAndFlags, HK_MEMORY_CLASS_ARRAY);
    if (ancestorsA.m_capacityAndFlags >= 0)
        hkThreadMemory_deallocateChunk(hkThreadMemory_getCurrent(), ancestorsA.m_data,
                                       4 * ancestorsA.m_capacityAndFlags, HK_MEMORY_CLASS_ARRAY);
    return commonParent;
}
