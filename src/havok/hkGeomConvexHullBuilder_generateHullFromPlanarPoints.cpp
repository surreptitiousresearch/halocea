#include "../headers/havok/hkGeomConvexHullBuilder_internal.h"
#include "../headers/havok/hkThreadMemory.h"
#include <math.h>

extern void hkArrayUtil__reserveMore(void *array, int sizeElem);

/* --- scalar helpers reconstructed from the VMX vsubfp / vmsum3fp / rsqrt / permute
   cross sequences. All operate on the xyz lanes of an hkVector4. --------------- */
static void ghp_sub3(const hkVector4 *a, const hkVector4 *b, float o[3])
{ o[0]=a->m_quad.___u0.v[0]-b->m_quad.___u0.v[0];
  o[1]=a->m_quad.___u0.v[1]-b->m_quad.___u0.v[1];
  o[2]=a->m_quad.___u0.v[2]-b->m_quad.___u0.v[2]; }
static float ghp_dot3(const float a[3], const hkVector4 *b)
{ return a[0]*b->m_quad.___u0.v[0]+a[1]*b->m_quad.___u0.v[1]+a[2]*b->m_quad.___u0.v[2]; }
static void ghp_cross3(const float a[3], const float b[3], float o[3])
{ o[0]=a[1]*b[2]-a[2]*b[1]; o[1]=a[2]*b[0]-a[0]*b[2]; o[2]=a[0]*b[1]-a[1]*b[0]; }
static void ghp_normalize3(float v[3])
{
    float l2 = v[0] * v[0] + v[1] * v[1] + v[2] * v[2];
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

/* Emit an in-plane edge plane: normal = normalize(dir x planeNormal), offset chosen
   so the plane passes through `through`. Stored as (nx,ny,nz,-d). */
static void ghp_emitEdgePlane(hkArray<hkVector4> *out, const float dir[3], const float planeN[3],
                              const hkVector4 *through)
{
    float n[3]; float d; int idx;
    ghp_cross3(planeN, dir, n);
    ghp_normalize3(n);
    if (out->m_size == (out->m_capacityAndFlags & 0x3FFFFFFF))
        hkArrayUtil__reserveMore(&out->m_data, 16);
    idx = out->m_size++;
    d = n[0]*through->m_quad.___u0.v[0] + n[1]*through->m_quad.___u0.v[1] + n[2]*through->m_quad.___u0.v[2];
    out->m_data[idx].m_quad.___u0.v[0] = n[0];
    out->m_data[idx].m_quad.___u0.v[1] = n[1];
    out->m_data[idx].m_quad.___u0.v[2] = n[2];
    out->m_data[idx].m_quad.___u0.v[3] = -d;
}

/* hkGeomConvexHullBuilder::generateHullFromPlanarPoints
   Slow-path builder for a set of coplanar points. It emits the front and back face
   planes (planeEqn and -planeEqn), then gift-wraps (Jarvis march) the points within
   the plane: starting from the extreme point along an in-plane tangent, it repeatedly
   selects the next boundary vertex by minimum turn angle, records each kept vertex in
   usedVertices, and appends the in-plane side plane for every hull edge.

   DEVIATION: the binary's VMX tangent construction (permute-based cross products),
   Newton rsqrt normalisation and quadrant-folded angle metric are reconstructed as
   scalar cross/dot/atan2-free turn selection. The temporary per-vertex "used" byte
   array is a hkThreadMemory chunk. */
void hkGeomConvexHullBuilder_generateHullFromPlanarPoints(const hkVector4 *planeEqn, const hkVector4 *verticesIn,
                                                          int numVerts, hkArray<hkVector4> *usedVertices,
                                                          hkArray<hkVector4> *planeEquationsOut)
{
    hkThreadMemory *mem = hkThreadMemory_getCurrent();
    char *used;
    float planeN[3];
    float tangent[3];
    float search[3];
    int start, current, prev, i;
    float best;

    planeEquationsOut->m_size = 0;

    /* front + back face planes */
    if ((planeEquationsOut->m_capacityAndFlags & 0x3FFFFFFF) == 0)
        hkArrayUtil__reserveMore(&planeEquationsOut->m_data, 16);
    planeEquationsOut->m_data[planeEquationsOut->m_size++] = *planeEqn;
    if (planeEquationsOut->m_size == (planeEquationsOut->m_capacityAndFlags & 0x3FFFFFFF))
        hkArrayUtil__reserveMore(&planeEquationsOut->m_data, 16);
    {
        int idx = planeEquationsOut->m_size++;
        planeEquationsOut->m_data[idx].m_quad.___u0.v[0] = -planeEqn->m_quad.___u0.v[0];
        planeEquationsOut->m_data[idx].m_quad.___u0.v[1] = -planeEqn->m_quad.___u0.v[1];
        planeEquationsOut->m_data[idx].m_quad.___u0.v[2] = -planeEqn->m_quad.___u0.v[2];
        planeEquationsOut->m_data[idx].m_quad.___u0.v[3] = -planeEqn->m_quad.___u0.v[3];
    }

    used = (char *)hkThreadMemory_allocateChunk(mem, numVerts, HK_MEMORY_CLASS_ARRAY);
    for (i = 0; i < numVerts; ++i)
        used[i] = 0;

    planeN[0] = planeEqn->m_quad.___u0.v[0];
    planeN[1] = planeEqn->m_quad.___u0.v[1];
    planeN[2] = planeEqn->m_quad.___u0.v[2];

    /* Build an in-plane tangent perpendicular to the plane normal. When the normal is
       ~parallel to z pick the x axis, otherwise cross with z. (Matches the VMX branch
       on |1 - |n.z||.) */
    if (fabsf(1.0f - fabsf(planeN[2])) >= 0.000001f)
    {
        float z[3] = { 0.0f, 0.0f, 1.0f };
        ghp_cross3(planeN, z, tangent);
    }
    else
    {
        float x[3] = { 1.0f, 0.0f, 0.0f };
        ghp_cross3(planeN, x, tangent);
    }
    ghp_normalize3(tangent);

    /* extreme vertex along the tangent = hull start */
    start = -1;
    best = -3.40282e38f;
    for (i = 0; i < numVerts; ++i)
    {
        float d = tangent[0]*verticesIn[i].m_quad.___u0.v[0]
                + tangent[1]*verticesIn[i].m_quad.___u0.v[1]
                + tangent[2]*verticesIn[i].m_quad.___u0.v[2];
        if (d > best) { best = d; start = i; }
    }

    used[start] = 1;
    if (usedVertices->m_size == (usedVertices->m_capacityAndFlags & 0x3FFFFFFF))
        hkArrayUtil__reserveMore(&usedVertices->m_data, 16);
    usedVertices->m_data[usedVertices->m_size++] = verticesIn[start];

    /* gift-wrap: search direction starts along the tangent */
    search[0] = tangent[0]; search[1] = tangent[1]; search[2] = tangent[2];
    current = start;
    prev = start;

    for (;;)
    {
        int next = -1;
        float bestTurn = 3.40282e38f;
        ghp_normalize3(search);

        for (i = 0; i < numVerts; ++i)
        {
            float edge[3], perp[3], sideDist, along, turn;
            if (i == current)
                continue;
            ghp_sub3(&verticesIn[i], &verticesIn[current], edge);
            ghp_normalize3(edge);
            /* signed turn metric: fold about the search direction into [0,4) so that
               the most clockwise in-plane point wins (quadrant folding from the VMX). */
            along = search[0]*edge[0] + search[1]*edge[1] + search[2]*edge[2];
            ghp_cross3(search, edge, perp);
            sideDist = planeN[0]*perp[0] + planeN[1]*perp[1] + planeN[2]*perp[2];
            turn = 1.0f - along;
            if (sideDist < 0.0f)
                turn = 4.0f - turn;
            else if (sideDist == 0.0f && along < 0.0f)
                turn = 2.0f - turn;
            if (turn < bestTurn) { bestTurn = turn; next = i; }
        }

        if (current != start)
        {
            float dir[3];
            ghp_sub3(&verticesIn[current], &verticesIn[prev], dir);
            ghp_emitEdgePlane(planeEquationsOut, dir, planeN, &verticesIn[current]);
        }

        if (used[next])
            break;

        used[next] = 1;
        if (usedVertices->m_size == (usedVertices->m_capacityAndFlags & 0x3FFFFFFF))
            hkArrayUtil__reserveMore(&usedVertices->m_data, 16);
        usedVertices->m_data[usedVertices->m_size++] = verticesIn[next];

        ghp_sub3(&verticesIn[next], &verticesIn[current], search);
        ghp_normalize3(search);
        prev = current;
        current = next;
    }

    /* closing edge */
    {
        float dir[3];
        ghp_sub3(&verticesIn[current], &verticesIn[prev], dir);
        ghp_emitEdgePlane(planeEquationsOut, dir, planeN, &verticesIn[current]);
    }

    if (numVerts >= 0)
        hkThreadMemory_deallocateChunk(mem, used, numVerts & 0x3FFFFFFF, HK_MEMORY_CLASS_ARRAY);
}
