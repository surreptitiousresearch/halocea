/* ?setSimpleValuesFromChildren@hkdBreakableShape@@QAAXPBUhkdShapeInstanceInfo@@H@Z @0x83848D80 */
#include "../headers/havok/hkdBreakableShape.h"
#include "../headers/havok/hkdShapeInstanceInfo.h"

#define HK_REAL_MAX 3.402823166e38f /* 0x7F7FFFEE — init for the running minimum */
#define HK_COUNT_EPSILON 1.1920929e-7f

/* hkdBreakableShape::setSimpleValuesFromChildren — derive this compound shape's
   simple authoring values by aggregating its children, but only fill fields that
   the caller left at zero:
     m_minDestructionRadius <- min child radius (if currently 0)
     m_tensionLength        <- average child tension length (if currently 0)
     m_strength             <- average child strength, and relativeSubpieceStrength
                               defaults to 0.7 (if strength currently 0)
     m_qualityType          <- max child quality (always)
   DEVIATION: the original hand-unrolls the child loop by two with an odd-count
   tail; reconstructed as a single loop (identical result). */
void hkdBreakableShape_setSimpleValuesFromChildren(hkdBreakableShape *self,
                                                   const hkdShapeInstanceInfo *childBreakableShapes,
                                                   int numChildBreakableShapes)
{
    float minRadius = HK_REAL_MAX;
    float totalStrength = 0.0f;
    float totalTension = 0.0f;
    int   maxQuality = 0;
    float invCount;
    int i;

    for (i = 0; i < numChildBreakableShapes; ++i)
    {
        const hkdBreakableShape *shape = childBreakableShapes[i].m_shape;
        if (minRadius - shape->m_minDestructionRadius >= 0.0f) /* fsel: min */
            minRadius = shape->m_minDestructionRadius;
        totalStrength += shape->m_strength;
        totalTension  += shape->m_tensionLength;
        if (maxQuality <= shape->m_qualityType.m_storage)
            maxQuality = shape->m_qualityType.m_storage;
    }

    invCount = 1.0f / ((float)numChildBreakableShapes + HK_COUNT_EPSILON);

    if (self->m_minDestructionRadius == 0.0f)
        self->m_minDestructionRadius = minRadius;
    if (self->m_tensionLength == 0.0f)
        self->m_tensionLength = invCount * totalTension;
    if (self->m_strength == 0.0f)
    {
        self->m_strength = invCount * totalStrength;
        self->m_qualityType.m_storage = (unsigned char)maxQuality;
        self->m_relativeSubpieceStrength = 0.69999999f;
    }
    else
    {
        self->m_qualityType.m_storage = (unsigned char)maxQuality;
    }
}
