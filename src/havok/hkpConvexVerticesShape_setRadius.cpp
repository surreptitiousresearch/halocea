/* FUNCTION_INDEX entry: hkpConvexVerticesShape_setRadius @0x8291B808 (?setRadius@hkpConvexShape@@QAAXM@Z) */
/* hkpConvexVerticesShape::setRadius — setRadius is not overridden by
   hkpConvexVerticesShape (no `?setRadius@hkpConvexVerticesShape@@` exists); the
   function is the inherited hkpConvexShape::setRadius, whose out-of-line body is
   at 0x8291B808 and ends in the same `stfs f31, 0x10(r31)` this file
   reconstructs (m_radius, offset 0x10 — see src/headers/havok/hkpConvexShape.h).
   The two call sites this TU was reconstructed from are the RELEASE-inlined
   expansion inside hkpShapeCutterUtil::cut, at 0x83F975F4 and 0x83F97688, each a
   bare `stfs f31, 0x10(r3)` on the shape a create*ConvexVerticesShape call just
   returned.
   DEVIATION: the out-of-line body at 0x8291B808 opens with a `radius >= 0`
   hkError check (hkErrStream / hkOstream, "hkpConvexShape should not have a
   negative radius") that both inlined sites elide; this reconstruction models
   the inlined form and carries only the store. */
#include "../headers/havok/hkpConvexVerticesShape.h"

void hkpConvexVerticesShape_setRadius(hkpConvexVerticesShape *shape, float r)
{
    shape->m_radius = r; /* stfs f31, 0x10(r3) */
}
