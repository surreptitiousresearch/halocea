#include "../headers/havok/hkdShapeInstanceInfo.h"
#include "../headers/havok/hkdBreakableShape.h"
#include "../headers/havok/hkTransform.h"
#include "../headers/havok/hkArray.h"
#include "../headers/havok/hkThreadMemory.h"
#include "../headers/havok/hkReferencedObject.h"

/*
 * CAVEAT (deviations from the VMX128-dense decompile, per never-stop drain policy):
 *  - Every hkTransform copy the decompiler emitted as raw lvx128/stvx128 __asm
 *    blocks has been restored to a scalar `hkTransform` struct assignment. Disasm
 *    confirms the four column moves at offsets 0/16/32/48 (m_rotation.m_col0/1/2 +
 *    m_translation): the loop pointer r29 tracks &elem->m_transform.m_rotation.m_col2
 *    (elem+32) and the copies index it with r20=-16, r25=16, r22=32, r23=48.
 *  - The decompiler expressed element striding as `_R29 += 5` over hkVector4 (16b);
 *    5*16 == 80 == sizeof(hkdShapeInstanceInfo). Rendered as a plain element pointer.
 *  - Leaf test: disasm at 0x838442D4 is `lbz r11,0xD(r24); rlwinm r10,r11,0,30,30;
 *    cmplwi r10,1; beq`. rlwinm keeps bit 0x2 in place (does not shift down), so the
 *    compiled test is literally `(m_flags & 2) == 1`, which never holds (0x2 != 1).
 *    Kept verbatim for binary fidelity — the leaf path is reached only via m_type.
 *  - m_damage bytes read by the decompiler as HIBYTE(quad.v[1]) are the big-endian
 *    high byte at elem+68 == hkdShapeInstanceInfo::m_damage; rendered as the field.
 *  - The name says "recursive" but the body flattens exactly one level: it reads the
 *    moved-out source array and appends into the freshly-emptied `shapes`; appended
 *    entries are not re-scanned. Refcounting: leaf entries keep their moved ref;
 *    each expanded compound child gains a new ref and the compound itself is released.
 *  - hkThreadMemory_getCurrent() models the TLS deref *(hkThreadMemory**)(*(int*)r13
 *    + 704) per the established corpus convention.
 */

/* Boundaries -- Havok SDK / other reversed siblings, not re-sourced here. */
extern void hkArrayUtil__reserveMore(void *array, int elementSize); /* hkArrayUtil::_reserveMore */
extern void hkdBreakableShape_inheritSimpleValuesFromParent(hkdBreakableShape *self,
                                                            const hkdBreakableShape *parent);
extern void hkTransform_setMul(hkTransform *out, const hkTransform *a, const hkTransform *b); /* hkTransform::setMul */

/* Flatten a one-deep tree of hkdShapeInstanceInfo: simple shapes are carried across
   unchanged, compound shapes are replaced by their children (transformed into the
   compound's frame, damage propagated). */
void _flattenRecursiveCompounds(hkArray_hkdShapeInstanceInfo *shapes)
{
    /* Move the incoming array out, then rebuild `shapes` from empty. */
    int sourceSize = shapes->m_size;
    hkdShapeInstanceInfo *sourceData = shapes->m_data;
    int sourceCapacityAndFlags = shapes->m_capacityAndFlags;
    shapes->m_data = 0;
    shapes->m_size = 0;
    shapes->m_capacityAndFlags = 0x80000000; /* "don't free" flag while empty */

    if (sourceSize > 0)
    {
        hkdShapeInstanceInfo *elem = sourceData;
        int remaining = sourceSize;
        do
        {
            const hkdBreakableShape *shape = elem->m_shape;
            /* binary-verified (rlwinm/cmplwi @0x838442D8): the compiler really compares (flags & 2) == 1,
             * which is always false — dead do-not-flatten test kept verbatim */
            if (shape->m_type.m_storage == SHAPE_TYPE_SIMPLE || (shape->m_flags & FLAG_DO_NOT_FLATTEN_RECURSIVE_COMPOUNDS) == 1)
            {
                /* Leaf: append a verbatim copy (keeps the moved-in reference). */
                if (shapes->m_size == (shapes->m_capacityAndFlags & 0x3FFFFFFF))
                    hkArrayUtil__reserveMore(shapes, sizeof(hkdShapeInstanceInfo));

                int newIndex = shapes->m_size;
                hkdShapeInstanceInfo *dest = &shapes->m_data[newIndex];
                shapes->m_size = newIndex + 1;
                dest->m_transform = elem->m_transform;
                dest->m_shape = elem->m_shape;
                dest->m_damage = elem->m_damage;
                dest->m_runtimeInfo = elem->m_runtimeInfo;
            }
            else
            {
                /* Compound: expand one level, re-parenting each child. */
                int appended = 0;
                if (shape->m_children.m_size > 0)
                {
                    int childIndex = 0;
                    do
                    {
                        hkdShapeInstanceInfo *child = &shape->m_children.m_data[childIndex];
                        hkdBreakableShape *childShape = (hkdBreakableShape *)child->m_shape;
                        hkdBreakableShape_inheritSimpleValuesFromParent(childShape, shape);
                        hkReferencedObject_addReference((hkReferencedObject *)childShape);

                        hkTransform childWorldTransform;
                        hkTransform_setMul(&childWorldTransform, &elem->m_transform, &child->m_transform);

                        const hkdBreakableShape *childShapePtr = child->m_shape;
                        unsigned int childDamage = child->m_damage;
                        unsigned char mergedDamage = elem->m_damage;
                        if (mergedDamage <= childDamage)
                            mergedDamage = (unsigned char)childDamage;

                        hkdShapeInstanceInfo_RuntimeInfo runtimeInfo;
                        runtimeInfo.m_forceFixed = 0;
                        runtimeInfo.m_distanceToDestructionPoint =
                            child->m_runtimeInfo.m_distanceToDestructionPoint;
                        runtimeInfo.m_oldChildIdx = (unsigned short)-1;

                        if (shapes->m_size == (shapes->m_capacityAndFlags & 0x3FFFFFFF))
                            hkArrayUtil__reserveMore(shapes, sizeof(hkdShapeInstanceInfo));

                        int newIndex = shapes->m_size;
                        hkdShapeInstanceInfo *dest = &shapes->m_data[newIndex];
                        shapes->m_size = newIndex + 1;
                        ++appended;
                        ++childIndex;

                        dest->m_transform = childWorldTransform;
                        dest->m_shape = childShapePtr;
                        dest->m_damage = mergedDamage;
                        dest->m_runtimeInfo = runtimeInfo;
                    } while (appended < shape->m_children.m_size);
                }
                hkReferencedObject_removeReference((hkReferencedObject *)shape);
            }
            --remaining;
            ++elem;
        } while (remaining);
    }

    if (sourceCapacityAndFlags >= 0)
        hkThreadMemory_deallocateChunk(hkThreadMemory_getCurrent(), sourceData,
                                       /* = 80 * capacity (element size 80) when storage flags clear */
                                       16 * ((sourceCapacityAndFlags & 0x3FFFFFFF) +
                                             4 * sourceCapacityAndFlags),
                                       HK_MEMORY_CLASS_ARRAY);
}
