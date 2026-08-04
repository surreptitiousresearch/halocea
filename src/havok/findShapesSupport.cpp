#include "../headers/havok/hkVector4.h"
#include "../headers/havok/hkTransform.h"
#include "../headers/havok/hkArray.h"
#include "../headers/havok/hkpConvexShape.h"

extern void hkVector4_setTransformedPos(hkVector4 *out, const hkTransform *t, const hkVector4 *v); /* hkVector4::setTransformedPos */

/* dot3 == vmsum3fp128: sum of the first three lane products. */
static float dot3(const hkVector4 *a, const hkVector4 *b)
{
    return a->m_quad.___u0.v[0] * b->m_quad.___u0.v[0] + a->m_quad.___u0.v[1] * b->m_quad.___u0.v[1] + a->m_quad.___u0.v[2] * b->m_quad.___u0.v[2];
}

/* GJK-style support mapping over a set of transformed convex shapes: for each
   shape, pull the direction into the shape's local frame (localDir = R^T *
   direction, one dot per rotation column), ask the shape for its supporting
   vertex, transform that vertex back to world space, and keep the world vertex
   whose projection on `direction` is largest. The winning point is written to
   `supportOut`.

   DEVIATION: the original is VMX128 -- vmsum3fp128 three-lane dot products and
   vmrghw lane merges assemble localDir as (dot(col0,dir), dot(col1,dir),
   dot(col2,dir), 0); reconstructed as scalar dot3 calls. The zero w-lane comes
   from merging in the hkQuadReal0000 constant. The getSupportingVertexImpl call
   is a virtual dispatch at vtable offset 0x38. */
void findShapesSupport(const hkArray<> *shapes,       /* hkArray<const hkpConvexShape*> */
                       const hkArray<> *transforms,   /* hkArray<hkTransform>          */
                       const hkVector4 *direction,
                       hkVector4 *supportOut)
{
    const hkpConvexShape **shapeData = (const hkpConvexShape **)shapes->m_data;
    const hkTransform *transformData = (const hkTransform *)transforms->m_data;
    int shapeCount = shapes->m_size;
    hkVector4 bestSupport = { { 0.0f, 0.0f, 0.0f, 0.0f } };
    float bestProjection = 0.0f;
    int i;

    for (i = 0; i < shapeCount; ++i)
    {
        const hkTransform *transform = &transformData[i];
        const hkpConvexShape *shape = shapeData[i];
        hkVector4 localDir;
        hkVector4 localSupport;
        hkVector4 worldSupport;
        hkpConvexShape_getSupportingVertexImpl_t getSupportingVertex;
        float projection;

        /* localDir = rotation^T * direction */
        localDir.m_quad.___u0.v[0] = dot3(&transform->m_rotation.m_col0, direction);
        localDir.m_quad.___u0.v[1] = dot3(&transform->m_rotation.m_col1, direction);
        localDir.m_quad.___u0.v[2] = dot3(&transform->m_rotation.m_col2, direction);
        localDir.m_quad.___u0.v[3] = 0.0f;

        getSupportingVertex =
            (hkpConvexShape_getSupportingVertexImpl_t)((void **)shape->base.__vftable)[0x38 / sizeof(void *)];
        getSupportingVertex(shape, &localDir, &localSupport);

        hkVector4_setTransformedPos(&worldSupport, transform, &localSupport);

        projection = dot3(direction, &worldSupport);
        if (i == 0 || projection > bestProjection)
        {
            bestProjection = projection;
            bestSupport = worldSupport;
        }
    }

    *supportOut = bestSupport;
}
