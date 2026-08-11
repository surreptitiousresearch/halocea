/* FUNCTION_INDEX entry: sortEdges @0x8384F848 (_sortEdges) */
#include "../headers/havok/hkdGeometry_HalfEdge.h"

/* sortEdges — strict-weak "less" comparator over hkdGeometry::HalfEdge, ordering
   edges lexicographically by (m_a, m_b, m_face, m_neighbouringFace). Used as the
   predicate for the half-edge quicksort in hkdGeometry::checkConsistency so that
   the two directed half-edges of a shared edge become adjacent. Returns nonzero
   when a sorts strictly before b. */
int sortEdges(const HalfEdge *a, const HalfEdge *b)
{
    if (a->m_a != b->m_a)
        return a->m_a < b->m_a;
    if (a->m_b != b->m_b)
        return a->m_b < b->m_b;
    if (a->m_face != b->m_face)
        return a->m_face < b->m_face;
    return a->m_neighbouringFace < b->m_neighbouringFace;
}
