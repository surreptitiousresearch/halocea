/* addToAabb @ 0x83608750 — scale a local-space point by `scaling`, transform it to
 * world space through `localToWorld`, and grow `aabb` to include the result
 * (component-wise min into m_min, max into m_max, across all 4 lanes).
 *
 * The original is VMX (lvx/vminfp/vmaxfp/setTransformedPos); reproduced here as
 * portable per-lane scalar min/max. hkVector4::setTransformedPos is a Havok
 * boundary call. */

#include "headers/havok/hkAabb.h"
#include "headers/havok/hkTransform.h"
#include "headers/havok/hkVector4.h"

extern void hkVector4_setTransformedPos(hkVector4 *out, const hkTransform *t, const hkVector4 *pos); /* hkVector4::setTransformedPos */

void addToAabb(hkAabb *aabb, const hkTransform *localToWorld, const float *v, const hkVector4 *scaling)
{
    hkVector4 local;
    hkVector4 world;
    int lane;

    local.m_quad.___u0.vector4_f32[0] = v[0] * scaling->m_quad.___u0.vector4_f32[0];
    local.m_quad.___u0.vector4_f32[1] = v[1] * scaling->m_quad.___u0.vector4_f32[1];
    local.m_quad.___u0.vector4_f32[2] = v[2] * scaling->m_quad.___u0.vector4_f32[2];
    local.m_quad.___u0.vector4_f32[3] = 0.0f;

    hkVector4_setTransformedPos(&world, localToWorld, &local);

    for ( lane = 0; lane < 4; ++lane )
    {
        float w = world.m_quad.___u0.vector4_f32[lane];
        if ( w < aabb->m_min.m_quad.___u0.vector4_f32[lane] )
            aabb->m_min.m_quad.___u0.vector4_f32[lane] = w;
        if ( w > aabb->m_max.m_quad.___u0.vector4_f32[lane] )
            aabb->m_max.m_quad.___u0.vector4_f32[lane] = w;
    }
}
