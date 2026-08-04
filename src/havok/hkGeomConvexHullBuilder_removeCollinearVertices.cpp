#include "../headers/havok/hkGeomConvexHullBuilder_internal.h"
#include <math.h>

/* --- scalar 3-vector helpers (reconstructed from the VMX subtract / vmsum3fp /
   rsqrt-Newton / cross-via-permute sequences). --------------------------------- */
static void rcv_sub3(const hkVector4 *a, const hkVector4 *b, float out[3])
{
    out[0] = a->m_quad.___u0.v[0] - b->m_quad.___u0.v[0];
    out[1] = a->m_quad.___u0.v[1] - b->m_quad.___u0.v[1];
    out[2] = a->m_quad.___u0.v[2] - b->m_quad.___u0.v[2];
}
static void rcv_normalize3(float v[3])
{
    float len2 = v[0] * v[0] + v[1] * v[1] + v[2] * v[2];
    if (len2 == 0.0f)
    {
        v[0] = v[1] = v[2] = 0.0f;   /* vsel against the ==0 mask -> zero */
    }
    else
    {
        float inv = 1.0f / sqrtf(len2);   /* DEVIATION: rsqrtefp + 2 Newton steps */
        v[0] *= inv; v[1] *= inv; v[2] *= inv;
    }
}
static float rcv_dot3(const float a[3], const float b[3])
{
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}
static float rcv_crossLen2(const float a[3], const float b[3])
{
    float cx = a[1] * b[2] - a[2] * b[1];
    float cy = a[2] * b[0] - a[0] * b[2];
    float cz = a[0] * b[1] - a[1] * b[0];
    return cx * cx + cy * cy + cz * cz;
}

/* hkGeomConvexHullBuilder::removeCollinearVertices
   Flags (w-lane = 1) vertices that are collinear with two others and drops them.
   For every triple (A=i, B=j, C=k) with i<j<k, it forms the normalised edge
   directions and, for each of the three vertices in turn (A, then C, then B), tests
   whether that vertex lies between the other two: its two incident edge directions
   are anti-parallel when the squared cross-product length is below degenerateTolerance
   AND the dot product is negative. The first such vertex found in a triple is flagged.
   Already-flagged vertices are skipped, and removeFlaggedVertices compacts them out. */
void hkGeomConvexHullBuilder_removeCollinearVertices(hkArray<hkVector4> *vertices, float degenerateTolerance)
{
    int size = vertices->m_size;
    int i, j, k;

    for (i = 0; i < size; ++i)
        vertices->m_data[i].m_quad.___u0.v[3] = 0.0f;

    for (i = 0; i < size; ++i)
    {
        for (j = i + 1; j < size; ++j)
        {
            for (k = j + 1; k < size; ++k)
            {
                hkVector4 *A = &vertices->m_data[i];
                hkVector4 *B = &vertices->m_data[j];
                hkVector4 *C = &vertices->m_data[k];
                float ab[3], ac[3], cb[3];

                if (A->m_quad.___u0.v[3] == 1.0f) continue;
                if (B->m_quad.___u0.v[3] == 1.0f) continue;
                if (C->m_quad.___u0.v[3] == 1.0f) continue;

                rcv_sub3(A, B, ab);   /* A - B */
                rcv_sub3(A, C, ac);   /* A - C */
                rcv_sub3(C, B, cb);   /* C - B */
                rcv_normalize3(ab);
                rcv_normalize3(ac);
                rcv_normalize3(cb);

                /* A between B and C: edges A->B (ab) and A->C (ac) anti-parallel */
                if (rcv_crossLen2(ab, ac) < degenerateTolerance && rcv_dot3(ab, ac) < 0.0f)
                {
                    A->m_quad.___u0.v[3] = 1.0f;
                    continue;
                }
                /* C between A and B: edges C->A (-ac) and C->B (cb) anti-parallel */
                if (rcv_crossLen2(ac, cb) < degenerateTolerance && -rcv_dot3(cb, ac) < 0.0f)
                {
                    C->m_quad.___u0.v[3] = 1.0f;
                    continue;
                }
                /* B between A and C: edges B->A (-ab) and B->C (-cb) anti-parallel */
                if (rcv_crossLen2(cb, ab) < degenerateTolerance && rcv_dot3(ab, cb) < 0.0f)
                {
                    B->m_quad.___u0.v[3] = 1.0f;
                }
            }
        }
    }

    hkGeomConvexHullBuilder_removeFlaggedVertices(vertices);
}
