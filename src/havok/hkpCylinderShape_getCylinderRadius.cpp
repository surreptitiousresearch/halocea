/* ?getCylinderRadius@hkpCylinderShape@@QBAMXZ @0x83604988 */
#include "../headers/havok/hkpCylinderShape.h"

/* hkpCylinderShape::getCylinderRadius — the cylinder's barrel radius (distinct
   from the inherited convex rounding radius). */
float hkpCylinderShape_getCylinderRadius(const hkpCylinderShape *self)
{
    return self->m_cylRadius;
}
