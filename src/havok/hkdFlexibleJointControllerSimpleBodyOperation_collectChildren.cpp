/* hkdFlexibleJointControllerSimpleBodyOperation_collectChildren @0x838551C8 */
#include "../headers/havok/hkdShapeInstanceInfo.h"
#include "../headers/havok/hkdBreakableShape.h"

extern void hkTransform_setMul(hkTransform *out, const hkTransform *a, const hkTransform *b); /* hkTransform::setMul */
extern void hkArrayUtil__reserveMore(void *array, int elementSize);                           /* hkArrayUtil::_reserveMore */

/* Flatten a breakable-shape subtree into `out`: for each child, compose its
   world transform (parent * child) and either recurse into flexible-joint
   sub-assemblies (jointType 1 with >1 children) or append a leaf instance with
   a default RuntimeInfo. DEVIATION: the decompiler suffered reg-alloc fusion
   (FLT_MAX merged with the output base pointer, and the shape/RuntimeInfo packed
   through a __PAIR64__); reconstructed to the semantic default RuntimeInfo
   (distanceToDestructionPoint = FLT_MAX, oldChildIdx = 0xFFFF, forceFixed = 0).
   The VMX stores were the world-transform copy. */
void hkdFlexibleJointControllerSimpleBodyOperation_collectChildren(
        const hkTransform *parentTransform,
        const hkdBreakableShape *parentShape,
        hkArray_hkdShapeInstanceInfo *out)
{
    hkdShapeInstanceInfo *children = (hkdShapeInstanceInfo *)parentShape->m_children.m_data;
    int count = parentShape->m_children.m_size;
    int i;

    for (i = 0; i < count; ++i)
    {
        hkTransform worldTransform;
        const hkdBreakableShape *childShape = children[i].m_shape;

        hkTransform_setMul(&worldTransform, parentTransform, &children[i].m_transform);

        if (childShape->m_flexibleJointType.m_storage == FLEXJOINT_CHILDREN && childShape->m_children.m_size > 1)
        {
            hkdFlexibleJointControllerSimpleBodyOperation_collectChildren(&worldTransform, childShape, out);
        }
        else
        {
            hkdShapeInstanceInfo *slot;

            if (out->m_size == (out->m_capacityAndFlags & 0x3FFFFFFF))
                hkArrayUtil__reserveMore(out, 80);

            slot = &out->m_data[out->m_size];
            slot->m_transform = worldTransform;
            slot->m_shape = childShape;
            slot->m_damage = 0;
            slot->m_runtimeInfo.m_distanceToDestructionPoint = 3.40282347e38f; /* FLT_MAX */
            slot->m_runtimeInfo.m_oldChildIdx = 0xFFFF;
            slot->m_runtimeInfo.m_forceFixed = 0;
            ++out->m_size;
        }
    }
}
