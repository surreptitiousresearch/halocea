/* ?getRadius@hkpConvexShape@@QBAMXZ @0x82CA7548 */
#include "../headers/havok/hkpConvexShape.h"

/* hkpConvexShape::getRadius — the extra convex (rounding) radius of the shape. */
float hkpConvexShape_getRadius(const hkpConvexShape *self)
{
    return self->m_radius;
}
