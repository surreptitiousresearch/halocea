/* ?getLeafShapes@hkdBreakableShape@@QBAXABVhkTransform@@AAV?$hkArray@UhkdShapeInstanceInfo@@@@@Z @0x83849B20 */
#include "../headers/havok/hkdBreakableShape.h"
#include "../headers/havok/hkdShapeInstanceInfo.h"
#include "../headers/havok/hkTransform.h"

extern void hkArrayUtil__reserveMore(void *array, int sizeElem);                       /* hkArrayUtil::_reserveMore */
extern void hkTransform_setMul(hkTransform *out, const hkTransform *a, const hkTransform *b); /* hkTransform::setMul */

#define HK_REAL_MAX 3.402823466e38f /* FLT_MAX sentinel */

/* hkdBreakableShape::getLeafShapes — collect the leaf shapes (those with no
   children) of this shape tree into leafShapesOut, each carrying its accumulated
   world transform. Internal nodes recurse with parentTransform * child.transform;
   leaves append themselves with the "unevaluated" runtime defaults. */
void hkdBreakableShape_getLeafShapes(hkdBreakableShape *self, const hkTransform *parentTransform,
                                     hkArray<hkdShapeInstanceInfo> *leafShapesOut)
{
    int numChildren = self->m_children.m_size;
    if (numChildren)
    {
        int i;
        for (i = 0; i < self->m_children.m_size; ++i)
        {
            hkTransform childWorld;
            hkTransform_setMul(&childWorld, parentTransform, &self->m_children.m_data[i].m_transform);
            hkdBreakableShape_getLeafShapes((hkdBreakableShape *)self->m_children.m_data[i].m_shape,
                                            &childWorld, leafShapesOut);
        }
    }
    else
    {
        hkdShapeInstanceInfo *info;
        if (leafShapesOut->m_size == (leafShapesOut->m_capacityAndFlags & 0x3FFFFFFF))
            hkArrayUtil__reserveMore(&leafShapesOut->m_data, 80);
        info = &leafShapesOut->m_data[leafShapesOut->m_size++];
        info->m_transform = *parentTransform;
        info->m_shape = self;
        info->m_damage = 0;
        info->m_runtimeInfo.m_distanceToDestructionPoint = HK_REAL_MAX;
        info->m_runtimeInfo.m_oldChildIdx = 0xFFFF;
        info->m_runtimeInfo.m_forceFixed = 0;
    }
}
