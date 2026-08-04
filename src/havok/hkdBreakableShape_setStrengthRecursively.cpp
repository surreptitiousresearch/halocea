#include "../headers/havok/hkdBreakableShape.h"
#include "../headers/havok/hkdShapeInstanceInfo.h"

extern double pow(double base, double exp);

/* hkdBreakableShape::setStrengthRecursively — set this shape's strength and
   relative-subpiece-strength, then (unless recalcChildren == 1, i.e. "do not
   recurse") propagate a volume-weighted strength down to each child.
   Child strength = m_strength * (childVolume/selfVolume)^(1 - relativeSubpieceStrength),
   or simply m_strength when relativeSubpieceStrength == 1. */
void hkdBreakableShape_setStrengthRecursively(hkdBreakableShape *self, float strength,
                                              float relativeSubpieceStrength, int recalcChildren)
{
    int i;
    self->m_strength = strength;
    self->m_relativeSubpieceStrength = relativeSubpieceStrength;

    if (recalcChildren == 1)
        return;

    for (i = 0; i < self->m_children.m_size; ++i)
    {
        hkdBreakableShape *child = (hkdBreakableShape *)self->m_children.m_data[i].m_shape;
        float childStrength = self->m_strength;
        if (self->m_relativeSubpieceStrength != 1.0f)
        {
            float exponent = 1.0f - self->m_relativeSubpieceStrength;
            float volumeFraction = child->m_massProps.m_volume / self->m_massProps.m_volume;
            childStrength = (float)pow(volumeFraction, exponent) * childStrength;
        }
        hkdBreakableShape_setStrengthRecursively(child, childStrength,
                                                 self->m_relativeSubpieceStrength, recalcChildren);
    }
}
