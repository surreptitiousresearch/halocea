#include "../headers/havok/hkpConvexVerticesShape.h"

/* hkpConvexVerticesShape::getPlaneEquations @ 0x8360F5F8
   Returns the hull's plane-equation array. */
const hkArray<hkVector4> *hkpConvexVerticesShape_getPlaneEquations(hkpConvexVerticesShape *self)
{
    return &self->m_planeEquations;
}
