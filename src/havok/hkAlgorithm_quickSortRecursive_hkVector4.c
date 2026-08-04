#include "../headers/havok/hkVector4.h"
#include "../headers/havok/hkBool.h"

/* hkAlgorithm::quickSortRecursive<hkVector4, hkBool(*)(hkVector4&,hkVector4&)>
   Hoare-partition quicksort over an hkVector4 array between indices [d,h], using a
   caller-supplied by-pointer comparator (returns *out = (lhs < rhs)). The pivot is
   the middle element; the shorter side recurses while the loop tail-iterates over
   the longer side (matching the binary's single self-recursion + outer while). */

typedef hkBool *(*hkVector4Less)(hkBool *result, hkVector4 *lhs, hkVector4 *rhs);

void hkAlgorithm_quickSortRecursive_hkVector4(hkVector4 *arr, int d, int h, hkVector4Less cmpLess)
{
    for (;;)
    {
        hkVector4 pivot = arr[(d + h) >> 1];
        int lo = d;
        int hi = h;
        hkBool t;

        do
        {
            while (cmpLess(&t, &arr[lo], &pivot)->m_bool)
                ++lo;
            while (cmpLess(&t, &pivot, &arr[hi])->m_bool)
                --hi;

            if (hi < lo)
                break;
            if (hi != lo)
            {
                hkVector4 tmp = arr[hi];
                arr[hi] = arr[lo];
                arr[lo] = tmp;
            }
            --hi;
            ++lo;
        }
        while (lo <= hi);

        if (d < hi)
            hkAlgorithm_quickSortRecursive_hkVector4(arr, d, hi, cmpLess);
        if (lo >= h)
            break;
        d = lo;
    }
}
