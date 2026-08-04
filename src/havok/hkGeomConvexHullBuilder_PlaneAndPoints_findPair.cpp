#include "../headers/havok/hkGeomConvexHullBuilder_internal.h"

/* hkGeomConvexHullBuilder::PlaneAndPoints::findPair
   Static helper: given the hull edge array base and two directed half-edges from each of
   two candidate planes (p11/p21 from plane p1, p12/p22 from plane p2), returns whether the
   two planes share a common undirected edge realized consistently across the two half-edge
   pairs. m_mirror indexes the opposite half-edge in edgeBase; the .m_vertex of the mirror
   edge is the far endpoint. The two hkGeomConvexHullBuilder::PlaneAndPoints operands (p1,p2)
   are unused by the current build but retained to match the ABI. Reconstructed verbatim from
   the decompiler's short-circuit boolean lattice. */

/* &edgeBase[e->m_mirror] — the opposite half-edge. */
#define HK_MIRROR_EDGE(base, e)  (&(base)[(e)->m_mirror])
/* far endpoint vertex of e. */
#define HK_MIRROR_VERTEX(base, e) ((base)[(e)->m_mirror].m_vertex)

hkBool *hkGeomConvexHullBuilder_PlaneAndPoints_findPair(hkBool *result, hkGeomEdge *edgeBase,
        hkGeomEdge *p11, hkGeomEdge *p21, hkGeomEdge *p12, hkGeomEdge *p22,
        hkGeomConvexHullBuilder_PlaneAndPoints *p1, hkGeomConvexHullBuilder_PlaneAndPoints *p2)
{
    int match1, match2;

    (void)p1;
    (void)p2;

    /* p11/p21 describe the same edge from opposite planes: same start vertex, distinct
       half-edges, distinct far endpoints. */
    match1 = (p11 != p21) && (p11->m_vertex == p21->m_vertex)
             && (HK_MIRROR_VERTEX(edgeBase, p11) != HK_MIRROR_VERTEX(edgeBase, p21));
    match2 = (p12 != p22) && (p12->m_vertex == p22->m_vertex)
             && (HK_MIRROR_VERTEX(edgeBase, p12) != HK_MIRROR_VERTEX(edgeBase, p22));

    result->m_bool = (char)(match1 && match2);

    if (result->m_bool)
    {
        int p22NotDupP11, p22NotDupP12, p22NotMirrorP11, p22NotMirrorP12, p22Consistent, p21NotDupP11, p21NotDupP12, p21NotMirrorP11, p21NotMirrorP12;

        p22NotDupP11 = (p11 == p22) || (p11->m_vertex != p22->m_vertex)
              || (HK_MIRROR_VERTEX(edgeBase, p22) != HK_MIRROR_VERTEX(edgeBase, p11));
        p22NotDupP12 = !p22NotDupP11 || (p12 == p22) || (p12->m_vertex != p22->m_vertex)
              || (HK_MIRROR_VERTEX(edgeBase, p22) != HK_MIRROR_VERTEX(edgeBase, p12));
        p22NotMirrorP11 = !p22NotDupP12 || (HK_MIRROR_VERTEX(edgeBase, p11) != p22->m_vertex)
              || (HK_MIRROR_VERTEX(edgeBase, p22) != p11->m_vertex)
              || (p22 == HK_MIRROR_EDGE(edgeBase, p11));
        p22NotMirrorP12 = !p22NotMirrorP11 || (HK_MIRROR_VERTEX(edgeBase, p12) != p22->m_vertex)
              || (HK_MIRROR_VERTEX(edgeBase, p22) != p12->m_vertex)
              || (p22 == HK_MIRROR_EDGE(edgeBase, p12));
        p22Consistent = p22NotMirrorP12;

        p21NotDupP11 = (p11 == p21) || (p11->m_vertex != p21->m_vertex)
              || (HK_MIRROR_VERTEX(edgeBase, p21) != HK_MIRROR_VERTEX(edgeBase, p11));
        p21NotDupP12 = !p21NotDupP11 || (p12 == p21) || (p12->m_vertex != p21->m_vertex)
              || (HK_MIRROR_VERTEX(edgeBase, p21) != HK_MIRROR_VERTEX(edgeBase, p12));
        p21NotMirrorP11 = !p21NotDupP12 || (HK_MIRROR_VERTEX(edgeBase, p11) != p21->m_vertex)
              || (HK_MIRROR_VERTEX(edgeBase, p21) != p11->m_vertex)
              || (p21 == HK_MIRROR_EDGE(edgeBase, p11));
        p21NotMirrorP12 = !p21NotMirrorP11 || (HK_MIRROR_VERTEX(edgeBase, p12) != p21->m_vertex)
              || (HK_MIRROR_VERTEX(edgeBase, p21) != p12->m_vertex)
              || (p21 == HK_MIRROR_EDGE(edgeBase, p12));

        result->m_bool = (char)(p22Consistent && p21NotMirrorP12);
    }

    return result;
}
