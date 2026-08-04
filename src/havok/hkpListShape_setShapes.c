#include "../headers/havok/hkpListShape.h"
#include "../headers/havok/hkAabb.h"

extern void hkArrayUtil__reserve(void *array, int numElem, int sizeElem); /* hkArrayUtil::_reserve */

/* Add a reference to `count` shapes stored `stride` bytes apart, starting at the
   first ChildInfo's m_shape pointer. Outside this slice. */
typedef struct hkReferencedObject hkReferencedObject;
extern void hkReferencedObject_addReferences(const hkReferencedObject *const *first, int count, int stride); /* hkReferencedObject::addReferences */

/* Recompute and cache the list shape's local-space AABB. Outside this slice. */
extern void hkpListShape_recalcAabbExtents(hkpListShape *self, hkAabb *scratch); /* hkpListShape::recalcAabbExtents */

/* hkpListShape::setShapes @ 0x83606558
   Populate the child-info array from `shapeArray`. `filterInfo` (when non-null)
   points to the parallel per-shape collision-filter values, addressed with the
   same element stride as `shapeArray`. `ref` controls whether the shapes are
   reference-counted. Finishes by recomputing the cached AABB. */
void hkpListShape_setShapes(hkpListShape *self, const hkpShape *const *shapeArray, int numShapes,
                            char *filterInfo, enum hkpShapeContainerReferencePolicy ref)
{
    hkpListShape_ChildInfoArray *childInfo = &self->m_childInfo;
    int cap = self->m_childInfo.m_capacityAndFlags & 0x3FFFFFFF;
    int i;

    if (cap < numShapes)
    {
        int grow = 2 * cap;
        if (numShapes >= grow)
            grow = numShapes;
        hkArrayUtil__reserve(&self->m_childInfo.m_data, grow, 16);
    }

    childInfo->m_size = numShapes;
    for (i = 0; i < numShapes; ++i)
    {
        const hkpShape *shape = shapeArray[i];
        if (shape)
        {
            unsigned int filter;
            childInfo->m_data[i].m_shape = shape;
            if (filterInfo)
                filter = *(unsigned int *)((char *)&shapeArray[i] + (filterInfo - (char *)shapeArray));
            else
                filter = 0;
            childInfo->m_data[i].m_collisionFilterInfo = filter;
            /* faithful to 0x83606608/0x83606614: field@12 <- numShapes, field@8 <- 0 */
            childInfo->m_data[i].m_numChildShapes = numShapes;
            childInfo->m_data[i].m_shapeSize = 0;
        }
    }

    if (ref == REFERENCE_POLICY_INCREMENT)
        /* 0x83606638 targets ?addReferences@hkReferencedObject@@SAXPBQBV1@HH@Z (0x838D2EC0) —
           the non-template base overload, not the addReferences<hkpShape> instantiation at
           0x83606868. C++ upcasts the hkpShape ** array element type implicitly; C cannot, so
           the conversion is explicit here. Safe because hkReferencedObject is hkpShape's first
           member and the callee is handed the 16-byte element stride outright. */
        hkReferencedObject_addReferences((const hkReferencedObject *const *)&childInfo->m_data->m_shape,
                                         self->m_childInfo.m_size, 16);

    {
        hkAabb scratch;
        hkpListShape_recalcAabbExtents(self, &scratch);
    }
}
