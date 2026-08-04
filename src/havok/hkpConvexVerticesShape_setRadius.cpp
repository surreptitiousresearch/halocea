/* hkpConvexVerticesShape::setRadius — inline-only setter reconstructed from its
   inlined call sites in hkpShapeCutterUtil::cut.
   Inlined stores: 0x83F975F4 and 0x83F97688, each `stfs f31, 0x10(r3)` writing the
   convex radius float into the freshly-created shape's m_radius field.
   DEVIATION: no out-of-line body exists in the binary (no `bl`); this definition
   reconstructs the single field store at m_radius (offset 0x10, inherited from
   hkpConvexShape — see src/headers/havok/hkpConvexShape.h). */
#include "../headers/havok/hkpConvexVerticesShape.h"

void hkpConvexVerticesShape_setRadius(hkpConvexVerticesShape *shape, float r)
{
    shape->m_radius = r; /* stfs f31, 0x10(r3) */
}
