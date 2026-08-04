#include "../headers/havok/hkpConvexVerticesShape.h"

/* hkpConvexVerticesShape::getNumCollisionSpheresImpl @ 0x8360F5D0
   Number of collision spheres equals the vertex count. */
int hkpConvexVerticesShape_getNumCollisionSpheresImpl(hkpConvexVerticesShape *self)
{
    return self->m_numVertices;
}
