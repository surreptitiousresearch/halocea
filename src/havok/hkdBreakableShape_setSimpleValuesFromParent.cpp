// hkdBreakableShape_setSimpleValuesFromParent @0x8384A0C0
#include "../headers/havok/hkdBreakableShape.h"
#include "../headers/havok/hkThreadMemory.h"
#include <math.h>
#include <string.h>

/* hkdBreakableShape::setSimpleValuesFromParent — inherit the simple scalar
   fracture properties from a parent shape onto a child sub-piece: flags,
   breaking-propagation rate, minimum destruction radius, quality type, and
   (only when the child has not set its own) connectivity type. The child's
   strength is the parent's strength scaled by (childVolume/parentVolume) raised
   to an exponent when the parent uses sub-piece strength (< 1); the property
   array is deep-copied; tension length is inherited.
   DEVIATION: the PPC decompiler surfaced the pow() exponent as a phantom FP
   argument register; the DB mangle (?...@@QAAXPBV1@@Z) and disasm 0x8384A0C0
   (only r3/r4 incoming, no f1) show a 2-parameter function. The exponent is the
   computed value (1 - parent->m_relativeSubpieceStrength) (disasm @0x8384A128:
   f2 = fsubs 1.0, relativeSubpieceStrength), not a caller argument. The property
   copy uses a flat 16-byte element move; the original additionally rebases an
   in-place pointer inside each hkpProperty::m_value, which is preserved by
   keeping the element self-relative offset identical after the realloc. */
void hkdBreakableShape_setSimpleValuesFromParent(hkdBreakableShape *self,
                                                 const hkdBreakableShape *parent)
{
    float strength;
    float refVolume;
    int parentCount;

    self->m_flags = parent->m_flags;
    self->m_breakingPropagationRate = parent->m_breakingPropagationRate;
    self->m_minDestructionRadius = parent->m_minDestructionRadius;
    self->m_qualityType.m_storage = parent->m_qualityType.m_storage;
    if (self->m_connectivityType.m_storage == 0)
        self->m_connectivityType.m_storage = parent->m_connectivityType.m_storage;

    strength = parent->m_strength;
    if (parent->m_relativeSubpieceStrength != 1.0f)
    {
        float ratio = self->m_massProps.m_volume / parent->m_massProps.m_volume;
        strength = (float)pow((double)ratio, (double)(1.0f - parent->m_relativeSubpieceStrength)) * strength;
    }
    self->m_strength = strength;
    self->m_relativeSubpieceStrength = parent->m_relativeSubpieceStrength;

    refVolume = parent->m_referenceShapeVolume;
    if (refVolume == 0.0f)
        refVolume = parent->m_massProps.m_volume;
    self->m_referenceShapeVolume = refVolume;

    /* Deep-copy the property array (hkpProperty is 16 bytes). */
    parentCount = parent->m_properties.m_properties.m_size;
    if ((self->m_properties.m_properties.m_capacityAndFlags & 0x3FFFFFFF) < parentCount)
    {
        hkThreadMemory *mem = hkThreadMemory_getCurrent();
        int cap = self->m_properties.m_properties.m_capacityAndFlags;
        void *chunk;
        if ((cap & 0x80000000) == 0)
            hkThreadMemory_deallocateChunk(mem, self->m_properties.m_properties.m_data,
                                           16 * cap, HK_MEMORY_CLASS_ARRAY);
        chunk = hkThreadMemory_allocateChunk(mem, 16 * parentCount, HK_MEMORY_CLASS_ARRAY);
        self->m_properties.m_properties.m_data = (hkpProperty *)chunk;
        self->m_properties.m_properties.m_capacityAndFlags =
            (self->m_properties.m_properties.m_capacityAndFlags & 0x40000000) | parentCount;
    }
    self->m_properties.m_properties.m_size = parentCount;
    if (parentCount > 0)
        memcpy(self->m_properties.m_properties.m_data,
               parent->m_properties.m_properties.m_data, (size_t)parentCount * 16);

    self->m_tensionLength = parent->m_tensionLength;
}
