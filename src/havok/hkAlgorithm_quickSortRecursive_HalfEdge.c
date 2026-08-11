/* FUNCTION_INDEX entry: hkAlgorithm_quickSortRecursive_HalfEdge @0x838539C0 (??$quickSortRecursive@UHalfEdge@hkdGeometry@@P6A_NABU12@0@Z@hkAlgorithm@@YAXPAUHalfEdge@hkdGeometry@@HHP6A_NABU12@1@Z@Z) */
#include "../headers/havok/hkdGeometry_HalfEdge.h"

/* hkAlgorithm::quickSortRecursive<hkdGeometry::HalfEdge, bool(*)(const HalfEdge&,
   const HalfEdge&)> — the HalfEdge instantiation of Havok's in-place recursive
   quicksort. Median-index pivot with the classic two-pointer partition; the low
   partition recurses and the high partition is handled by tail iteration (the
   original tail-call is written here as a loop, matching the DB body). */
void hkAlgorithm_quickSortRecursive_HalfEdge(HalfEdge *pArr, int firstIndex, int lastIndex,
                                             int (*cmpLess)(const HalfEdge *, const HalfEdge *))
{
    for (;;)
    {
        int hi = lastIndex;
        int lo = firstIndex;
        HalfEdge pivot = pArr[(firstIndex + lastIndex) >> 1];

        do
        {
            while (cmpLess(&pArr[lo], &pivot))
                ++lo;
            while (cmpLess(&pivot, &pArr[hi]))
                --hi;

            if (hi < lo)
                break;

            if (hi != lo)
            {
                HalfEdge tmp = pArr[hi];
                pArr[hi] = pArr[lo];
                pArr[lo] = tmp;
            }
            --hi;
            ++lo;
        }
        while (lo <= hi);

        if (firstIndex < hi)
            hkAlgorithm_quickSortRecursive_HalfEdge(pArr, firstIndex, hi, cmpLess);

        if (lo >= lastIndex)
            break;
        firstIndex = lo;
    }
}
