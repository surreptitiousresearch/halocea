#include "../headers/havok/hkdCompoundBreakableShape.h"
#include "../headers/havok/hkdShapeInstanceInfo.h"
#include "../headers/havok/hkpMassElement.h"
#include "../headers/havok/hkpMassProperties.h"
#include "../headers/havok/hkArray.h"
#include "../headers/havok/hkThreadMemory.h"

/* Deeper callee kept as a boundary extern (2-level descent limit): accumulate a
   parent's mass properties from an array of placed child mass elements. */
extern void hkpInertiaTensorComputer_combineMassProperties(const hkArray<hkpMassElement> *elements,
                                                           hkpMassProperties *out); /* hkpInertiaTensorComputer::combineMassProperties */

/* hkdCompoundBreakableShape::recalcMassPropertiesFromChildren — rebuild this
   compound's mass properties by combining each child's mass properties placed at
   its child transform.
   DEVIATION: the inline thread-stack bump/pop allocation of the scratch element
   array is restored to hkThreadMemory allocateStack/deallocateStack, and the
   4x-unrolled copy loop is collapsed to a single element loop. */
void hkdCompoundBreakableShape_recalcMassPropertiesFromChildren(hkdCompoundBreakableShape *self)
{
    hkThreadMemory *threadMemory = hkThreadMemory_getCurrent();
    int numChildren = self->base.m_children.m_size;
    hkArray<hkpMassElement> massElements;
    hkpMassElement *elementStorage;
    int i;

    massElements.m_data = 0;
    massElements.m_size = 0;
    massElements.m_capacityAndFlags = 0x80000000;

    elementStorage = (hkpMassElement *)hkThreadMemory_allocateStack(threadMemory, 144 * numChildren);
    massElements.m_data = elementStorage;
    massElements.m_size = numChildren;
    massElements.m_capacityAndFlags = numChildren | 0x80000000; /* stack storage, not owned */

    for (i = 0; i < numChildren; ++i)
    {
        hkdShapeInstanceInfo *child = &self->base.m_children.m_data[i];
        massElements.m_data[i].m_transform = child->m_transform;
        massElements.m_data[i].m_properties = child->m_shape->m_massProps;
    }

    hkpInertiaTensorComputer_combineMassProperties(&massElements, &self->base.m_massProps);

    hkThreadMemory_deallocateStack(threadMemory, elementStorage);

    /* Fallback free path for a heap-spilled array (unreached while the stack
       allocation keeps the DONT_DEALLOCATE flag set). */
    if (massElements.m_capacityAndFlags >= 0)
        hkThreadMemory_deallocateChunk(threadMemory, massElements.m_data,
                                       16 * ((massElements.m_capacityAndFlags & 0x3FFFFFFF) +
                                             8 * massElements.m_capacityAndFlags),
                                       HK_MEMORY_CLASS_ARRAY);
}
