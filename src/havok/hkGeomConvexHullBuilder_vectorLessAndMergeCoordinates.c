#include "../headers/havok/hkVector4.h"
#include "../headers/havok/hkBool.h"

/* hkGeomConvexHullBuilder::vectorLessAndMergeCoordinates
   Strict lexicographic "less than" over the (x,y,z) coordinates, used as the sort
   comparator that brings coincident plane equations next to each other before the
   x-sorted weld pass. Returns *result = (a < b) with x as primary, y secondary and
   z tertiary key. Returns the result pointer (Havok by-value hkBool ABI). */
hkBool *hkGeomConvexHullBuilder_vectorLessAndMergeCoordinates(hkBool *result, hkVector4 *a, hkVector4 *b)
{
    float ax = a->m_quad.___u0.v[0];
    float bx = b->m_quad.___u0.v[0];

    if (ax < bx || (ax == bx && a->m_quad.___u0.v[1] < b->m_quad.___u0.v[1]))
    {
        result->m_bool = 1;
    }
    else
    {
        char less = 1;
        if (ax != bx
            || a->m_quad.___u0.v[1] != b->m_quad.___u0.v[1]
            || a->m_quad.___u0.v[2] >= b->m_quad.___u0.v[2])
        {
            less = 0;
        }
        result->m_bool = less;
    }
    return result;
}
