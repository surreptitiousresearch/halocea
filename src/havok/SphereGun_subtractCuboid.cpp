/* SphereGun_subtractCuboid @0x8385E410 */
#include "../headers/havok/hkAabb.h"
#include "../headers/havok/hkVector4.h"
#include "../headers/havok/hkArray.h"
#include "../headers/havok/hkReferencedObject.h"

typedef struct hkpShape hkpShape;

extern void hkpShapeCutterUtil_cut(const hkpShape *shape, const hkVector4 *plane, float tolerance,
                                   const hkpShape **outsideOut, const hkpShape **insideOut); /* hkpShapeCutterUtil::cut */
extern void hkArrayUtil__reserveMore(void *array, int elementSize);                          /* hkArrayUtil::_reserveMore */

/* Subtract an axis-aligned cuboid from a shape by cutting it, in turn, with each
   of the box's six face planes. Every cut splits the current piece into an
   outside sliver (collected into outsideShapes) and an inside remainder that is
   fed into the next cut; the piece surviving all six planes is the intersection
   with the box and is returned through insideShapeOut. The six inward face planes
   (decoded from the VMX plane-build) are {+-1 axis, box extent}. */
void SphereGun_subtractCuboid(const hkAabb *aabb, const hkpShape *shape,
                              const hkpShape **insideShapeOut, hkArray<> *outsideShapes) /* hkArray<const hkpShape*> */
{
    const float *mn = aabb->m_min.m_quad.___u0.v;
    const float *mx = aabb->m_max.m_quad.___u0.v;
    hkVector4 planes[6];
    const hkpShape *current;
    const hkpShape *outsidePiece;
    const hkpShape *insidePiece;
    int i;

    planes[0].m_quad.___u0.v[0] = -1.0f; planes[0].m_quad.___u0.v[1] =  0.0f; planes[0].m_quad.___u0.v[2] =  0.0f; planes[0].m_quad.___u0.v[3] =  mn[0];
    planes[1].m_quad.___u0.v[0] =  1.0f; planes[1].m_quad.___u0.v[1] =  0.0f; planes[1].m_quad.___u0.v[2] =  0.0f; planes[1].m_quad.___u0.v[3] = -mx[0];
    planes[2].m_quad.___u0.v[0] =  0.0f; planes[2].m_quad.___u0.v[1] = -1.0f; planes[2].m_quad.___u0.v[2] =  0.0f; planes[2].m_quad.___u0.v[3] =  mn[1];
    planes[3].m_quad.___u0.v[0] =  0.0f; planes[3].m_quad.___u0.v[1] =  1.0f; planes[3].m_quad.___u0.v[2] =  0.0f; planes[3].m_quad.___u0.v[3] = -mx[1];
    planes[4].m_quad.___u0.v[0] =  0.0f; planes[4].m_quad.___u0.v[1] =  0.0f; planes[4].m_quad.___u0.v[2] = -1.0f; planes[4].m_quad.___u0.v[3] =  mn[2];
    planes[5].m_quad.___u0.v[0] =  0.0f; planes[5].m_quad.___u0.v[1] =  0.0f; planes[5].m_quad.___u0.v[2] =  1.0f; planes[5].m_quad.___u0.v[3] = -mx[2];

    hkReferencedObject_addReference((hkReferencedObject *)shape);
    current = shape;

    for (i = 0; i < 6; ++i)
    {
        if (!current)
            break;
        hkpShapeCutterUtil_cut(current, &planes[i], 0.0f, &outsidePiece, &insidePiece);
        if (outsidePiece)
        {
            if (outsideShapes->m_size == (outsideShapes->m_capacityAndFlags & 0x3FFFFFFF))
                hkArrayUtil__reserveMore(outsideShapes, 4);
            ((const hkpShape **)outsideShapes->m_data)[outsideShapes->m_size++] = outsidePiece;
        }
        hkReferencedObject_removeReference((hkReferencedObject *)current);
        current = insidePiece;
    }

    *insideShapeOut = current;
}
