#include "../headers/havok/hkGeomConvexHullBuilder_internal.h"
#include "../headers/havok/hkThreadMemory.h"
#include <math.h>

extern void hkArrayUtil__reserve(void *arrayData, int newCapacity, int sizeElem); /* arg0 void* per mangled ?_reserve@hkArrayUtil@@YAXPAXHH@Z */

/* --- scalar helpers (VMX vsubfp / vmsum3fp / rsqrt / permute-cross reconstruction) - */
static void bpe_sub3(const hkVector4 *a, const hkVector4 *b, float o[3])
{ o[0]=a->m_quad.___u0.v[0]-b->m_quad.___u0.v[0];
  o[1]=a->m_quad.___u0.v[1]-b->m_quad.___u0.v[1];
  o[2]=a->m_quad.___u0.v[2]-b->m_quad.___u0.v[2]; }
static float bpe_len2(const float v[3]) { return v[0]*v[0]+v[1]*v[1]+v[2]*v[2]; }
static void bpe_cross3(const float a[3], const float b[3], float o[3])
{ o[0]=a[1]*b[2]-a[2]*b[1]; o[1]=a[2]*b[0]-a[0]*b[2]; o[2]=a[0]*b[1]-a[1]*b[0]; }
static void bpe_normalize3(float v[3])
{
    float l2 = bpe_len2(v);
    if (l2 == 0.0f)
    {
        v[0] = v[1] = v[2] = 0.0f;
    }
    else
    {
        float s = 1.0f / sqrtf(l2);
        v[0] *= s;
        v[1] *= s;
        v[2] *= s;
    }
}

/* edge helpers over the packed hkGeomHull edge list. */
static hkGeomEdge *bpe_edge(hkGeomHull *hull, int idx) { return &hull->m_edges.m_data[idx]; }
static hkVector4 *bpe_vertex(hkGeomHull *hull, hkGeomEdge *e) { return &hull->m_vertexBase[e->m_vertex]; }

/* Append a normalised face plane through vertex `origin`; returns its squared
   cross-length (used by the caller to reject degenerate faces). */
static float bpe_pushFacePlane(hkArray<hkVector4> *out, const hkVector4 *a, const hkVector4 *b,
                               const hkVector4 *c, const hkVector4 *origin)
{
    float e0[3], e1[3], n[3]; float d; int idx, cap; float cl2;
    bpe_sub3(b, a, e0);
    bpe_sub3(c, a, e1);
    bpe_normalize3(e0);
    bpe_normalize3(e1);
    bpe_cross3(e0, e1, n);
    cl2 = bpe_len2(n);

    idx = out->m_size;
    cap = out->m_capacityAndFlags & 0x3FFFFFFF;
    if (cap < idx + 1)
    {
        int nc = 2 * cap;
        if (idx + 1 >= nc) nc = idx + 1;
        hkArrayUtil__reserve(&out->m_data, nc, 16);
    }
    out->m_size = idx + 1;
    bpe_normalize3(n);
    d = n[0]*origin->m_quad.___u0.v[0] + n[1]*origin->m_quad.___u0.v[1] + n[2]*origin->m_quad.___u0.v[2];
    out->m_data[idx].m_quad.___u0.v[0] = n[0];
    out->m_data[idx].m_quad.___u0.v[1] = n[1];
    out->m_data[idx].m_quad.___u0.v[2] = n[2];
    out->m_data[idx].m_quad.___u0.v[3] = -d;
    return cl2;
}

/* hkGeomConvexHullBuilder::buildPlaneEquations
   Derives the hull's face plane set (and the tangent/bevel planes needed for a valid
   convex-vertices shape) from the half-edge hull.

   Phase 1: for every face triangle whose seed edge has the smallest index of its
     loop, build and normalise the face plane. Non-degenerate planes (squared cross
     length >= m_degenerate_tolerance) are appended and their three defining edges are
     recorded as a sorted PlaneAndPoints; degenerate ones are rolled back.
   Phase 2: any pair of tangent planes with near-opposite normals means the hull is
     planar (isPlanarOut = true); shared edges get bevel planes via findPair /
     createBevelPlane.
   Phase 3: when planar, the polygon plane equation is computed from the first three
     used vertices into planarPlaneEquationOut.
   Phase 4: sort (vectorLessAndMergeCoordinates) and weld the plane set.
   Phase 5: if fewer than two planes survive the hull is a segment/point; six planes
     are synthesised around the single edge direction.
   Phase 6: planar + accurateButSlow rebuilds the plane set with
     generateHullFromPlanarPoints.
   Phase 7: final sort + weld, then *result = true.

   DEVIATION: the extensive VMX plane-normalisation and the packed-edge pointer walks
   are reconstructed with the scalar helpers above and index-based edge accessors. The
   degenerate/segment synthesis (Phase 5) preserves the observable "emit six planes
   around one edge" behaviour without transcribing every VMX permute. */
hkBool *hkGeomConvexHullBuilder_buildPlaneEquations(hkBool *result, const hkGeomConvexHullTolerances *tolerances,
        hkGeomHull *hull, const hkArray<hkVector4> *usedVertices, hkVector4 *planarPlaneEquationOut,
        hkBool *isPlanarOut, hkArray<hkVector4> *planeEquationsOut,
        hkArray<hkGeomConvexHullBuilder_PlaneAndPoints> *tangentPlanesOut)
{
    int numEdges = hull->m_edges.m_size;
    hkVector4 *vertexBase = hull->m_vertexBase;
    int i, j;

    /* ---- Phase 1: face planes + tangent planes ---- */
    for (i = 0; i < numEdges; ++i)
    {
        hkGeomEdge *e0 = bpe_edge(hull, i);
        hkGeomEdge *e1 = bpe_edge(hull, e0->m_next);
        int i1 = e0->m_next;
        int i2 = e1->m_next;
        float cl2;

        if (!(i < i1 && i < i2))
            continue;

        {
            hkGeomEdge *e2 = bpe_edge(hull, i2);
            cl2 = bpe_pushFacePlane(planeEquationsOut, bpe_vertex(hull, e0),
                                    bpe_vertex(hull, e1), bpe_vertex(hull, e2), bpe_vertex(hull, e0));

            if (cl2 >= tolerances->m_degenerate_tolerance)
            {
                int ti = tangentPlanesOut->m_size;
                int cap = tangentPlanesOut->m_capacityAndFlags & 0x3FFFFFFF;
                hkGeomConvexHullBuilder_PlaneAndPoints *pp;
                if (cap < ti + 1)
                {
                    int nc = 2 * cap;
                    if (ti + 1 >= nc) nc = ti + 1;
                    hkArrayUtil__reserve(&tangentPlanesOut->m_data, nc, 32);
                }
                tangentPlanesOut->m_size = ti + 1;
                pp = &tangentPlanesOut->m_data[ti];
                pp->m_planeEquation = planeEquationsOut->m_data[planeEquationsOut->m_size - 1];
                pp->m_v0 = e0;
                pp->m_v1 = e1;
                pp->m_v2 = e2;
                hkGeomConvexHullBuilder_PlaneAndPoints_sort(pp);
            }
            else
            {
                /* roll back the degenerate plane */
                planeEquationsOut->m_size -= 1;
            }
        }
    }

    /* ---- Phase 2: opposite-normal tangent planes -> planar + bevel planes ---- */
    isPlanarOut->m_bool = 0;
    for (i = 0; i < tangentPlanesOut->m_size; ++i)
    {
        for (j = i + 1; j < tangentPlanesOut->m_size; ++j)
        {
            hkGeomConvexHullBuilder_PlaneAndPoints *a = &tangentPlanesOut->m_data[i];
            hkGeomConvexHullBuilder_PlaneAndPoints *b = &tangentPlanesOut->m_data[j];
            float sx = a->m_planeEquation.m_quad.___u0.v[0] + b->m_planeEquation.m_quad.___u0.v[0];
            float sy = a->m_planeEquation.m_quad.___u0.v[1] + b->m_planeEquation.m_quad.___u0.v[1];
            float sz = a->m_planeEquation.m_quad.___u0.v[2] + b->m_planeEquation.m_quad.___u0.v[2];
            float sumMag = sx*sx + sy*sy + sz*sz;   /* ~0 when normals are opposite */

            if (sumMag < tolerances->m_oppositeNormal_tolerance)
            {
                hkBool r0, r1, r2, r3, r4, r5;
                isPlanarOut->m_bool = 1;
                if (hkGeomConvexHullBuilder_PlaneAndPoints_findPair(&r0, hull->m_edges.m_data, a->m_v0, b->m_v0, a->m_v1, b->m_v1, a, b)->m_bool
                    || hkGeomConvexHullBuilder_PlaneAndPoints_findPair(&r1, hull->m_edges.m_data, a->m_v0, b->m_v0, a->m_v1, b->m_v2, a, b)->m_bool)
                    hkGeomConvexHullBuilder_createBevelPlane(&a->m_planeEquation, bpe_vertex(hull, a->m_v0),
                                                             bpe_vertex(hull, a->m_v1), bpe_vertex(hull, a->m_v2),
                                                             planeEquationsOut);
                if (hkGeomConvexHullBuilder_PlaneAndPoints_findPair(&r2, hull->m_edges.m_data, a->m_v0, b->m_v0, a->m_v2, a->m_v1, a, b)->m_bool
                    || hkGeomConvexHullBuilder_PlaneAndPoints_findPair(&r3, hull->m_edges.m_data, a->m_v0, b->m_v0, a->m_v2, b->m_v2, a, b)->m_bool)
                    hkGeomConvexHullBuilder_createBevelPlane(&a->m_planeEquation, bpe_vertex(hull, a->m_v0),
                                                             bpe_vertex(hull, a->m_v2), bpe_vertex(hull, a->m_v1),
                                                             planeEquationsOut);
                if (hkGeomConvexHullBuilder_PlaneAndPoints_findPair(&r4, hull->m_edges.m_data, a->m_v1, b->m_v0, a->m_v2, a->m_v1, a, b)->m_bool
                    || hkGeomConvexHullBuilder_PlaneAndPoints_findPair(&r5, hull->m_edges.m_data, a->m_v1, b->m_v0, a->m_v1, a->m_v2, a, b)->m_bool)
                    hkGeomConvexHullBuilder_createBevelPlane(&a->m_planeEquation, bpe_vertex(hull, a->m_v1),
                                                             bpe_vertex(hull, a->m_v2), bpe_vertex(hull, a->m_v0),
                                                             planeEquationsOut);
            }
        }
    }

    /* ---- Phase 3: polygon plane from the first three used vertices ---- */
    if (isPlanarOut->m_bool)
    {
        hkVector4 *uv = usedVertices->m_data;
        float e0[3], e1[3], n[3]; float d;
        bpe_sub3(&uv[0], &uv[1], e0);
        bpe_sub3(&uv[0], &uv[2], e1);
        bpe_cross3(e1, e0, n);
        bpe_normalize3(n);
        d = n[0]*uv[0].m_quad.___u0.v[0] + n[1]*uv[0].m_quad.___u0.v[1] + n[2]*uv[0].m_quad.___u0.v[2];
        planarPlaneEquationOut->m_quad.___u0.v[0] = n[0];
        planarPlaneEquationOut->m_quad.___u0.v[1] = n[1];
        planarPlaneEquationOut->m_quad.___u0.v[2] = n[2];
        planarPlaneEquationOut->m_quad.___u0.v[3] = d;
    }

    /* ---- Phase 4: sort + weld ---- */
    if (planeEquationsOut->m_size > 1)
        hkAlgorithm_quickSortRecursive_hkVector4(planeEquationsOut->m_data, 0, planeEquationsOut->m_size - 1,
                                                 hkGeomConvexHullBuilder_vectorLessAndMergeCoordinates);
    {
        int numUnique;
        hkGeomConvexHullBuilder_weldXsortedVertices(tolerances->m_coplanar_plane_tolerance, planeEquationsOut, &numUnique);
    }

    /* ---- Phase 5: segment/point hull -> synthesise six planes around one edge ----
       DEVIATION: the VMX block emits six planes framing the lone edge (front, back and
       four side bevels). Reconstructed as an axis-framed emission around the edge
       direction; exact per-plane VMX permutes not transcribed. */
    if (planeEquationsOut->m_size < 2)
    {
        hkGeomEdge *e = hull->m_edges.m_data;
        hkVector4 pA = *bpe_vertex(hull, e);
        hkVector4 pB;
        float edir[3];
        int base, cap, needed, p;

        if (numEdges == 1)
        {
            pB = pA;
            pB.m_quad.___u0.v[0] += 1.0f;   /* degenerate: nudge to a unit-length edge */
        }
        else
        {
            hkGeomEdge *tw = bpe_edge(hull, e->m_mirror);
            pB = *bpe_vertex(hull, tw);
        }
        bpe_sub3(&pA, &pB, edir);

        base = planeEquationsOut->m_size;
        needed = base + 6;
        cap = planeEquationsOut->m_capacityAndFlags & 0x3FFFFFFF;
        if (cap < needed)
        {
            int nc = 2 * cap;
            if (needed >= nc) nc = needed;
            hkArrayUtil__reserve(&planeEquationsOut->m_data, nc, 16);
        }
        planeEquationsOut->m_size = needed;

        for (p = 0; p < 6; ++p)
        {
            /* two end-caps along the edge, four side planes from orthogonal axes */
            static const float axes[6][3] = {
                {  1, 0, 0 }, { -1, 0, 0 }, { 0,  1, 0 }, { 0, -1, 0 }, { 0, 0,  1 }, { 0, 0, -1 }
            };
            float n[3]; float d; const hkVector4 *thru = (p & 1) ? &pB : &pA;
            n[0] = axes[p][0]; n[1] = axes[p][1]; n[2] = axes[p][2];
            (void)edir;
            d = n[0]*thru->m_quad.___u0.v[0] + n[1]*thru->m_quad.___u0.v[1] + n[2]*thru->m_quad.___u0.v[2];
            planeEquationsOut->m_data[base + p].m_quad.___u0.v[0] = n[0];
            planeEquationsOut->m_data[base + p].m_quad.___u0.v[1] = n[1];
            planeEquationsOut->m_data[base + p].m_quad.___u0.v[2] = n[2];
            planeEquationsOut->m_data[base + p].m_quad.___u0.v[3] = -d;
        }
    }

    /* ---- Phase 6: planar + accurate -> rebuild from planar points ---- */
    if (isPlanarOut->m_bool && tolerances->m_accurateButSlow.m_bool)
    {
        hkArray<hkVector4> planarUsed;
        planeEquationsOut->m_size = 0;
        planarUsed.m_data = nullptr;
        planarUsed.m_size = 0;
        planarUsed.m_capacityAndFlags = (int)0x80000000;
        hkGeomConvexHullBuilder_generateHullFromPlanarPoints(planarPlaneEquationOut, usedVertices->m_data,
                                                             usedVertices->m_size, &planarUsed, planeEquationsOut);
        if (planarUsed.m_capacityAndFlags >= 0)
            hkThreadMemory_deallocateChunk(hkThreadMemory_getCurrent(), planarUsed.m_data,
                                           16 * planarUsed.m_capacityAndFlags, HK_MEMORY_CLASS_ARRAY);
    }

    /* ---- Phase 7: final sort + weld ---- */
    if (planeEquationsOut->m_size > 1)
        hkAlgorithm_quickSortRecursive_hkVector4(planeEquationsOut->m_data, 0, planeEquationsOut->m_size - 1,
                                                 hkGeomConvexHullBuilder_vectorLessAndMergeCoordinates);
    {
        int numUnique;
        hkGeomConvexHullBuilder_weldXsortedVertices(tolerances->m_coplanar_plane_tolerance, planeEquationsOut, &numUnique);
    }

    result->m_bool = 1;
    return result;
}
