/* hkpWrapperShape_getChildShape — out-of-line reconstruction of the inlined
   single-child accessor used by hkpShapeCutterUtil::cut (@0x83F97520).
   No hkpWrapperShape class exists in the DB; cut() inlined a per-shape-type raw
   load of the child pointer. This shim recovers that dispatch:
     - HK_SHAPE_CONVEX_TRANSLATE (11): child @0x18   disasm @0x83F977B4 lwz r29,0x18(r31)
     - HK_SHAPE_CONVEX_TRANSFORM (12): child @0x18   disasm @0x83F978D4 lwz r29,0x18(r31)
     - HK_SHAPE_TRANSFORM        (15): child @0x14   disasm @0x83F976D8 lwz r29,0x14(r31)
   The two convex-transform cases share hkpConvexTransformShapeBase (hkpConvexShape
   base = 20 bytes, container @20, child ptr @24=0x18); the plain transform shape
   has only the 16-byte hkpShape base (container @16, child ptr @20=0x14).
   DEVIATION: the binary reads these through a single overlaid load site; expressed
   here as a type-switch over the concrete layouts so the differing offsets stay
   type-anchored rather than raw. */
#include "../headers/havok/hkpWrapperShape.h"

const hkpShape *hkpWrapperShape_getChildShape(const hkpShape *shape)
{
    switch (shape->m_type.m_storage)
    {
    case HK_SHAPE_CONVEX_TRANSLATE:
    case HK_SHAPE_CONVEX_TRANSFORM:
        return ((const hkpConvexTransformShapeBase *)shape)->m_childShape.m_childShape;
    case HK_SHAPE_TRANSFORM:
        return ((const hkpTransformShapeChildView *)shape)->m_childShape.m_childShape;
    default:
        return 0;
    }
}
