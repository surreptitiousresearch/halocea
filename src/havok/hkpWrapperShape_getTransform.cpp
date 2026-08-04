/* hkpWrapperShape_getTransform — out-of-line reconstruction of the inlined per-shape-type
   transform accessor used by hkpShapeCutterUtil::cut (@0x83F97520). There is no
   hkpWrapperShape class in the DB; cut() handled the three single-child "wrapper" shapes
   with separate inline arms:
     - HK_SHAPE_TRANSFORM        (15): transform stored @0x30   disasm @0x83F976E4 addi r28,r31,0x30
     - HK_SHAPE_CONVEX_TRANSFORM (12): transform stored @0x20   disasm @0x83F978E0 addi r28,r31,0x20
     - HK_SHAPE_CONVEX_TRANSLATE (11): NO stored transform — the binary synthesizes
       hkTransformIdentity(rotation) + the shape's translation(@0x20) into a PER-FRAME STACK
       buffer (var_90), disasm @0x83F977B0..F8 (lvx128/stvx128 of the identity columns +
       the shape+0x20 translation).

   DEVIATION from a naive pure accessor: the CONVEX_TRANSLATE arm has no stored transform to
   return a pointer to, so the caller passes a per-call `scratch` buffer (modelling the
   binary's stack var_90). This preserves the per-frame lifetime that cut's recursion relies
   on — a shared static buffer would be clobbered by a nested CONVEX_TRANSLATE reached through
   hkpShapeCutterUtil_cut3 before the outer frame re-reads the transform. */
#include "../headers/havok/hkpWrapperShape.h"           /* hkpTransformShapeChildView (m_transform @0x30), hkpConvexTranslateShape */
#include "../headers/havok/hkpConvexTransformShape.h"   /* hkpConvexTransformShape: m_transform @0x20 */

extern const hkTransform hkTransformIdentity;

const hkTransform *hkpWrapperShape_getTransform(const hkpShape *shape, hkTransform *scratch)
{
    switch (shape->m_type.m_storage)
    {
    case HK_SHAPE_TRANSFORM:
        return &((const hkpTransformShapeChildView *)shape)->m_transform; /* stored @0x30 */
    case HK_SHAPE_CONVEX_TRANSFORM:
        return &((const hkpConvexTransformShape *)shape)->m_transform;    /* stored @0x20 */
    case HK_SHAPE_CONVEX_TRANSLATE:
    default:
        /* synthesize: identity rotation + the shape's stored translation (@0x20) */
        scratch->m_rotation = hkTransformIdentity.m_rotation;
        scratch->m_translation = ((const hkpConvexTranslateShape *)shape)->m_translation;
        return scratch;
    }
}
