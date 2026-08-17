/* ?postFilterVertices@hkGeomConvexHullBuilder@@SAXAAVhkGeomHull@@HHABUhkGeomConvexHullTolerances@@AAVhkBool@@@Z @0x83FDA140 */
#include "../headers/havok/hkGeomConvexHullBuilder_internal.h"
#include <math.h>

/* --- scalar 3-vector helpers (reconstructed from VMX vsubfp / vmsum3fp / cross-via-vpermwi
   / vrsqrtefp+Newton+vsel sequences). ---------------------------------------------------- */
static void pfv_sub3(const hkVector4 *a, const hkVector4 *b, float out[3])
{
    out[0] = a->m_quad.___u0.v[0] - b->m_quad.___u0.v[0];
    out[1] = a->m_quad.___u0.v[1] - b->m_quad.___u0.v[1];
    out[2] = a->m_quad.___u0.v[2] - b->m_quad.___u0.v[2];
}
static float pfv_dot3(const float a[3], const float b[3])
{
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}
static void pfv_cross(const float a[3], const float b[3], float out[3])
{
    out[0] = a[1] * b[2] - a[2] * b[1];
    out[1] = a[2] * b[0] - a[0] * b[2];
    out[2] = a[0] * b[1] - a[1] * b[0];
}
static float pfv_crossLen2(const float a[3], const float b[3])
{
    float c[3];
    pfv_cross(a, b, c);
    return c[0] * c[0] + c[1] * c[1] + c[2] * c[2];
}
static void pfv_normalize3(float v[3]) /* DEVIATION: original vrsqrtefp + 2 Newton + vsel(==0) */
{
    float len2 = v[0] * v[0] + v[1] * v[1] + v[2] * v[2];
    if (len2 == 0.0f)
    {
        v[0] = v[1] = v[2] = 0.0f;
    }
    else
    {
        float inv = 1.0f / sqrtf(len2);
        v[0] *= inv; v[1] *= inv; v[2] *= inv;
    }
}

/* hkGeomConvexHullBuilder::postFilterVertices
   When m_postFilter is enabled, removes vertices that are redundant on the current hull:
     Phase B (collinear-on-edge): for every hull edge whose two endpoints are active, any
       active vertex that projects onto the edge line and lies between (or extends) the
       endpoints is flagged for removal; the two true extreme endpoints of each collinear
       chain are kept.
     Phase C (coplanar-around-vertex): for every active vertex, the incident face normals are
       gathered by walking each edge fan; if all incident faces share (near-)the same normal
       direction, the vertex is interior to a flat region and is flagged for removal.
   Any flag set marks vertsHaveChanged. Vertex "active/removed" state lives in the w-lane of
   hull->m_vertexBase[i]. m_info (the edge's 4th u16) is used as a visited marker. */
void hkGeomConvexHullBuilder_postFilterVertices(hkGeomHull *hull, int startVertex, int endVertex,
        const hkGeomConvexHullTolerances *tolerances, hkBool *vertsHaveChanged)
{
    hkVector4 *vb;
    hkInplaceArray<hkGeomEdge, 128> *edges;
    hkThreadMemory *mem;
    int i;

    vertsHaveChanged->m_bool = 0;
    if (!tolerances->m_postFilter.m_bool)
        return;

    vb = hull->m_vertexBase;
    edges = &hull->m_edges;
    mem = hkThreadMemory_getCurrent();

    /* clear visited flags */
    for (i = 0; i < edges->m_size; ++i)
        edges->m_data[i].m_info = 0;

    /* ---- Phase B: flag collinear-on-edge interior vertices ---- */
    for (i = 0; i < edges->m_size; ++i)
    {
        hkGeomEdge *edge = &edges->m_data[i];
        int vA, vB;
        float edgeVec[3], edgeLenSq;
        hkArray<int> cand;

        if (edge->m_info == 1)
            continue;
        edge->m_info = 1;
        edges->m_data[edge->m_mirror].m_info = 1;

        vA = edge->m_vertex;
        vB = edges->m_data[edge->m_mirror].m_vertex;

        pfv_sub3(&vb[vB], &vb[vA], edgeVec);
        edgeLenSq = pfv_dot3(edgeVec, edgeVec);

        cand.m_data = nullptr;
        cand.m_size = 0;
        cand.m_capacityAndFlags = 0x80000000;

        if (vB != vA
            && vb[vA].m_quad.___u0.v[3] == 0.0f
            && vb[vB].m_quad.___u0.v[3] == 0.0f)
        {
            float minT = 0.000001f;
            float maxT = edgeLenSq;
            int farNeg = vA;
            int farPos = vB;
            int j;

            for (j = startVertex; j < endVertex + 1; ++j)
            {
                float d[3], t, crossLen2;
                if (vA == j || vB == j || vb[j].m_quad.___u0.v[3] != 0.0f)
                    continue;

                pfv_sub3(&vb[j], &vb[vA], d);
                t = pfv_dot3(edgeVec, d);
                crossLen2 = pfv_crossLen2(edgeVec, d);

                if (crossLen2 < tolerances->m_degenerate_tolerance)
                {
                    int pushVal;
                    if (t >= minT)
                    {
                        if (t <= maxT)
                        {
                            pushVal = j;           /* interior of the chain */
                        }
                        else
                        {
                            pushVal = farPos;      /* j extends past the positive end */
                            maxT = t;
                            farPos = j;
                        }
                    }
                    else
                    {
                        pushVal = farNeg;          /* j extends past the negative end */
                        minT = t;
                        farNeg = j;
                    }
                    if (cand.m_size == (cand.m_capacityAndFlags & 0x3FFFFFFF))
                        hkArrayUtil__reserveMore(&cand.m_data, 4);
                    cand.m_data[cand.m_size++] = pushVal;
                }
            }

            for (j = 0; j < cand.m_size; ++j)
            {
                int idx = cand.m_data[j];
                if (idx != farNeg && idx != farPos)
                {
                    vb[idx].m_quad.___u0.v[3] = 1.0f;
                    vertsHaveChanged->m_bool = 1;
                }
            }
        }

        if (cand.m_capacityAndFlags >= 0)
            hkThreadMemory_deallocateChunk(mem, cand.m_data, 4 * cand.m_capacityAndFlags,
                                           HK_MEMORY_CLASS_ARRAY);
    }

    /* clear visited flags again for Phase C */
    for (i = 0; i < edges->m_size; ++i)
        edges->m_data[i].m_info = 0;

    /* ---- Phase C: flag coplanar-around-vertex interior vertices ---- */
    if (endVertex - startVertex > 2 && edges->m_size > 2)
    {
        int v;
        for (v = startVertex; v < endVertex + 1; ++v)
        {
            hkArray<hkVector4> normals;
            int k;

            if (vb[v].m_quad.___u0.v[3] != 0.0f)
                continue;

            normals.m_data = nullptr;
            normals.m_size = 0;
            normals.m_capacityAndFlags = 0x80000000;

            /* gather one normalized face normal per corner of every fan incident to v */
            for (k = 0; k < edges->m_size; ++k)
            {
                int fanStart, cur;
                if (edges->m_data[k].m_info == 1 || edges->m_data[k].m_vertex != v)
                    continue;

                edges->m_data[k].m_info = 1;
                fanStart = edges->m_data[k].m_mirror;
                cur = fanStart;
                do
                {
                    int nextIdx = edges->m_data[cur].m_next;
                    int P = edges->m_data[cur].m_vertex;
                    int Q = edges->m_data[nextIdx].m_vertex;
                    int nnIdx = edges->m_data[nextIdx].m_next;
                    int R = edges->m_data[nnIdx].m_vertex;
                    float e1[3], e2[3], normal[3];

                    edges->m_data[nextIdx].m_info = 1;

                    pfv_sub3(&vb[P], &vb[Q], e1);
                    pfv_sub3(&vb[R], &vb[Q], e2);
                    pfv_cross(e1, e2, normal);
                    pfv_normalize3(normal);

                    if (normals.m_size == (normals.m_capacityAndFlags & 0x3FFFFFFF))
                        hkArrayUtil__reserveMore(&normals.m_data, 16);
                    normals.m_data[normals.m_size].m_quad.___u0.v[0] = normal[0];
                    normals.m_data[normals.m_size].m_quad.___u0.v[1] = normal[1];
                    normals.m_data[normals.m_size].m_quad.___u0.v[2] = normal[2];
                    normals.m_data[normals.m_size].m_quad.___u0.v[3] = 0.0f;
                    ++normals.m_size;

                    cur = edges->m_data[nextIdx].m_mirror;
                } while (cur != fanStart);
            }

            if (normals.m_size > 0)
            {
                float sum[3] = {0.0f, 0.0f, 0.0f};
                float sumLen2;
                int j;

                for (j = 0; j < normals.m_size; ++j)
                {
                    sum[0] += normals.m_data[j].m_quad.___u0.v[0];
                    sum[1] += normals.m_data[j].m_quad.___u0.v[1];
                    sum[2] += normals.m_data[j].m_quad.___u0.v[2];
                }
                sumLen2 = pfv_dot3(sum, sum);

                if (sumLen2 > tolerances->m_coplanar_vertices_tolerance)
                {
                    int keep = 0;
                    pfv_normalize3(sum);
                    for (j = 0; j < normals.m_size; ++j)
                    {
                        float dot = normals.m_data[j].m_quad.___u0.v[0] * sum[0]
                                  + normals.m_data[j].m_quad.___u0.v[1] * sum[1]
                                  + normals.m_data[j].m_quad.___u0.v[2] * sum[2];
                        if (dot < 1.0f - tolerances->m_coplanar_vertices_tolerance)
                        {
                            keep = 1;
                            break;
                        }
                    }
                    if (!keep)
                    {
                        vb[v].m_quad.___u0.v[3] = 1.0f;
                        vertsHaveChanged->m_bool = 1;
                    }
                }
            }

            if (normals.m_capacityAndFlags >= 0)
                hkThreadMemory_deallocateChunk(mem, normals.m_data, 16 * normals.m_capacityAndFlags,
                                               HK_MEMORY_CLASS_ARRAY);
        }
    }
}
