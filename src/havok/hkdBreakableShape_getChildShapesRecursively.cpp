/* ?getChildShapesRecursively@hkdBreakableShape@@QBAXABVhkTransform@@AAV?$hkArray@UhkdShapeInstanceInfo@@@@@Z @0x8384A548 */
#include "../headers/havok/hkdBreakableShape.h"
#include "../headers/havok/hkdShapeInstanceInfo.h"
#include "../headers/havok/hkTransform.h"
#include "../headers/havok/hkInplaceArray.h"
#include "../headers/havok/hkThreadMemory.h"

extern void hkArrayUtil__reserveMore(void *array, int sizeElem);                            /* hkArrayUtil::_reserveMore */
extern void hkTransform_setMul(hkTransform *out, const hkTransform *a, const hkTransform *b); /* hkTransform::setMul */

#define HK_REAL_MAX 3.402823466e38f /* FLT_MAX sentinel for "not yet computed" */

/* hkdBreakableShape::getChildShapesRecursively — flatten the whole descendant
   tree of this shape into `shapes`, each entry carrying its accumulated world
   transform (rootTransform * ... * child.transform) and the max damage
   propagated down its path. An explicit work stack replaces recursion.
   DEVIATION: the VMX128 lvx/stvx hkTransform moves are restored to scalar struct
   assignment, and the inline thread-stack spill/free is expressed through the
   hkInplaceArray + guarded deallocateChunk it implements. */
void hkdBreakableShape_getChildShapesRecursively(hkdBreakableShape *self,
                                                 const hkTransform *rootTransform,
                                                 hkArray<hkdShapeInstanceInfo> *shapes)
{
    hkInplaceArray<hkdShapeInstanceInfo, 32> pending; /* nodes still to expand */
    RuntimeInfo defaultRuntime;

    shapes->m_size = 0;

    defaultRuntime.m_distanceToDestructionPoint = HK_REAL_MAX;
    defaultRuntime.m_oldChildIdx = 0xFFFF;
    defaultRuntime.m_forceFixed = 0;

    /* Seed the stack with this shape at the root transform. */
    pending.m_data = pending.m_storage;
    pending.m_size = 1;
    pending.m_capacityAndFlags = 0x80000020; /* inline capacity 32, storage not owned */
    pending.m_storage[0].m_transform = *rootTransform;
    pending.m_storage[0].m_shape = self;
    pending.m_storage[0].m_damage = 0;
    pending.m_storage[0].m_runtimeInfo = defaultRuntime;

    do
    {
        /* Pop the top node; snapshot its fields before the loop may reallocate
           the stack out from under `node`. */
        hkdShapeInstanceInfo *node = &pending.m_data[--pending.m_size];
        hkTransform nodeTransform = node->m_transform;
        const hkdBreakableShape *nodeShape = node->m_shape;
        unsigned char nodeDamage = node->m_damage;
        int numChildren = nodeShape->m_children.m_size;
        int i;

        for (i = 0; i < numChildren; ++i)
        {
            hkdShapeInstanceInfo *child = &nodeShape->m_children.m_data[i];
            hkTransform childWorld;
            unsigned char childDamage = child->m_damage;
            unsigned char propagatedDamage = nodeDamage;
            RuntimeInfo childRuntime;
            hkdShapeInstanceInfo *outInstance;

            hkTransform_setMul(&childWorld, &nodeTransform, &child->m_transform);
            if (nodeDamage <= childDamage)
                propagatedDamage = childDamage;

            childRuntime.m_forceFixed = 0;
            childRuntime.m_distanceToDestructionPoint = child->m_runtimeInfo.m_distanceToDestructionPoint;
            childRuntime.m_oldChildIdx = 0xFFFF;

            /* Emit this descendant. */
            if (shapes->m_size == (shapes->m_capacityAndFlags & 0x3FFFFFFF))
                hkArrayUtil__reserveMore(&shapes->m_data, 80);
            outInstance = &shapes->m_data[shapes->m_size++];
            outInstance->m_transform = childWorld;
            outInstance->m_shape = child->m_shape;
            outInstance->m_damage = propagatedDamage;
            outInstance->m_runtimeInfo = childRuntime;

            /* Push it too if it has children of its own. */
            if (child->m_shape->m_children.m_size)
            {
                hkdShapeInstanceInfo *pushed;
                if (pending.m_size == (pending.m_capacityAndFlags & 0x3FFFFFFF))
                    hkArrayUtil__reserveMore(&pending.m_data, 80);
                pushed = &pending.m_data[pending.m_size++];
                pushed->m_transform = childWorld;
                pushed->m_shape = child->m_shape;
                pushed->m_damage = propagatedDamage;
                pushed->m_runtimeInfo = childRuntime;
            }
        }
    } while (pending.m_size > 0);

    /* Free the stack's backing store if it spilled onto the heap. */
    if (pending.m_capacityAndFlags >= 0)
        hkThreadMemory_deallocateChunk(hkThreadMemory_getCurrent(), pending.m_data,
                                       16 * ((pending.m_capacityAndFlags & 0x3FFFFFFF) +
                                             4 * pending.m_capacityAndFlags),
                                       HK_MEMORY_CLASS_ARRAY);
}
