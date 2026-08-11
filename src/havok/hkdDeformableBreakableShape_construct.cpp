// FUNCTION_INDEX entry: hkdDeformableBreakableShape_construct @0x838644A8 (??0hkdDeformableBreakableShape@@QAA@PBVhkdBreakableShape@@PBUhkdShapeInstanceInfo@@HW4ConstructorFlags@hkdCompoundBreakableShape@@@Z)
#include "../headers/havok/hkdDeformableBreakableShape.h"
#include "../headers/havok/hkdShapeInstanceInfo.h"
#include "../headers/havok/hkBaseObject.h"
#include "../headers/havok/hkTransform.h"

extern void hkdCompoundBreakableShape_construct(hkdCompoundBreakableShape *self,
        const hkdBreakableShape *oldParent, const hkdShapeInstanceInfo *childBreakableShapes,
        int numChildBreakableShapes, int flags); /* hkdCompoundBreakableShape::hkdCompoundBreakableShape */
extern void hkArrayUtil__reserve(void *array, int numElem, int sizeElem); /* hkArrayUtil::_reserve */

/* The class's own vtable (data-segment global). */
extern hkBaseObject_vtbl hkdDeformableBreakableShape_vftable; /* hkdDeformableBreakableShape::`vftable' */

/* hkdDeformableBreakableShape::hkdDeformableBreakableShape — construct as a
   compound breakable shape (forcing the SET_CHILD_PARENT flag, bit 0), then
   override the vtable/type to DEFORMABLE and snapshot each child's authoring
   transform into m_origChildTransforms. */
void hkdDeformableBreakableShape_construct(hkdDeformableBreakableShape *self,
        const hkdBreakableShape *oldParent, const hkdShapeInstanceInfo *childBreakableShapes,
        int numChildBreakableShapes, int flags)
{
    int i;
    hkdCompoundBreakableShape_construct(&self->base, oldParent, childBreakableShapes,
                                        numChildBreakableShapes, flags | 1);
    ((hkBaseObject *)self)->__vftable = &hkdDeformableBreakableShape_vftable;
    self->m_origChildTransforms.m_data = 0;
    self->m_origChildTransforms.m_size = 0;
    self->m_origChildTransforms.m_capacityAndFlags = 0x80000000;
    self->base.base.m_type.m_storage = SHAPE_TYPE_DEFORMABLE;

    if ((self->m_origChildTransforms.m_capacityAndFlags & 0x3FFFFFFF) < numChildBreakableShapes)
    {
        int grow = 2 * (self->m_origChildTransforms.m_capacityAndFlags & 0x3FFFFFFF);
        if (numChildBreakableShapes >= grow)
            grow = numChildBreakableShapes;
        hkArrayUtil__reserve(&self->m_origChildTransforms.m_data, grow, (int)sizeof(hkTransform));
    }
    self->m_origChildTransforms.m_size = numChildBreakableShapes;
    for (i = 0; i < numChildBreakableShapes; ++i)
        self->m_origChildTransforms.m_data[i] = childBreakableShapes[i].m_transform;
}
