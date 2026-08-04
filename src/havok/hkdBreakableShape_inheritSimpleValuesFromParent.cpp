#include "../headers/havok/hkdBreakableShape.h"
#include "../headers/havok/hkdProperties.h"
#include "../headers/havok/hkArray.h"
#include "../headers/havok/hkThreadMemory.h"
#include <math.h>
#include <string.h>

/* hkArrayUtil::_reserveMore — type-erased grow-by-doubling (boundary sibling). */
extern void hkArrayUtil__reserveMore(void *array, int sizeElem);

/* hkdBreakableShape::inheritSimpleValuesFromParent — merge the parent's simple
   fracture values into this child piece. Differs from setSimpleValuesFromParent
   in the property handling: if the child already carries properties they are
   MERGED (only parent keys not already present are appended), otherwise the
   parent's property array is deep-copied. Each remaining scalar is inherited only
   when the child still holds its default (0, or 0.1 for the propagation rate);
   integrity type "3" is demoted to "2" when inherited. Strength, when unset, is
   the parent's strength scaled by (childVolume/parentVolume)^(1 - relativeSubpieceStrength)
   whenever the parent uses sub-piece strength (< 1).
   DEVIATION: the PPC decompiler surfaced the pow() exponent as a phantom FP
   argument; the DB mangle (?...@@QAAXPBV1@@Z) and disasm 0x8384A230 (only r3/r4
   incoming, no f1) show a 2-parameter function. The exponent is the computed
   value (1 - parent->m_relativeSubpieceStrength) (disasm @0x8384A4A4:
   f2 = fsubs 1.0, relativeSubpieceStrength), not a caller argument. The allocator
   is threaded through the raw per-thread TLS deref; expressed here via pow() and
   hkThreadMemory_getCurrent(). */
void hkdBreakableShape_inheritSimpleValuesFromParent(hkdBreakableShape *self,
                                                     const hkdBreakableShape *parent)
{
    self->m_flags = parent->m_flags;

    if (self->m_properties.m_properties.m_size)
    {
        /* Merge: append each parent property whose key the child lacks. */
        int i;
        for (i = 0; i < parent->m_properties.m_properties.m_size; ++i)
        {
            const hkpProperty *src = &parent->m_properties.m_properties.m_data[i];
            int found = 0;
            int j;
            for (j = 0; j < self->m_properties.m_properties.m_size; ++j)
            {
                if (self->m_properties.m_properties.m_data[j].m_key == src->m_key)
                {
                    found = 1;
                    break;
                }
            }
            if (!found)
            {
                hkpProperty *dst;
                if (self->m_properties.m_properties.m_size ==
                    (self->m_properties.m_properties.m_capacityAndFlags & 0x3FFFFFFF))
                {
                    hkArrayUtil__reserveMore(&self->m_properties.m_properties, 16);
                }
                dst = &self->m_properties.m_properties.m_data[self->m_properties.m_properties.m_size];
                dst->m_key = src->m_key;
                dst->m_alignmentPadding = src->m_alignmentPadding;
                dst->m_value.m_data = src->m_value.m_data;
                ++self->m_properties.m_properties.m_size;
            }
        }
    }
    else
    {
        /* Deep-copy the parent's property array (hkpProperty is 16 bytes). */
        int parentCount = parent->m_properties.m_properties.m_size;
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
    }

    if (self->m_breakingPropagationRate == 0.1f)
        self->m_breakingPropagationRate = parent->m_breakingPropagationRate;
    if (self->m_minDestructionRadius == 0.0f)
        self->m_minDestructionRadius = parent->m_minDestructionRadius;
    if (self->m_tensionLength == 0.0f)
        self->m_tensionLength = parent->m_tensionLength;
    if (!self->m_connectivityType.m_storage)
        self->m_connectivityType.m_storage = parent->m_connectivityType.m_storage;
    if (!self->m_qualityType.m_storage)
        self->m_qualityType.m_storage = parent->m_qualityType.m_storage;
    if (!self->m_integrityType.m_storage)
    {
        self->m_integrityType.m_storage = parent->m_integrityType.m_storage;
        if (parent->m_integrityType.m_storage == INTEGRITY_CHILDREN)
            self->m_integrityType.m_storage = INTEGRITY_ON;
    }

    if (self->m_strength == 0.0f)
    {
        float strength = parent->m_strength;
        if (parent->m_relativeSubpieceStrength != 1.0f)
        {
            float ratio = self->m_massProps.m_volume / parent->m_massProps.m_volume;
            strength = (float)((float)pow((double)ratio, (double)(1.0f - parent->m_relativeSubpieceStrength)) * strength);
        }
        self->m_strength = strength;
        self->m_relativeSubpieceStrength = parent->m_relativeSubpieceStrength;
    }

    {
        float refVolume = parent->m_referenceShapeVolume;
        if (refVolume == 0.0f)
            refVolume = parent->m_massProps.m_volume;
        self->m_referenceShapeVolume = refVolume;
    }
}
