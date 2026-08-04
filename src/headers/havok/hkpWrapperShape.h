#pragma once
/* hkpWrapperShape — synthetic abstraction over the three single-child "wrapper"
   shapes the reversed hkpShapeCutterUtil::cut (@0x83F97520) treats uniformly:
   HK_SHAPE_CONVEX_TRANSLATE (11), HK_SHAPE_CONVEX_TRANSFORM (12) and
   HK_SHAPE_TRANSFORM (15). There is no hkpWrapperShape class in the DB; the child
   pointer lives at a type-dependent offset which the compiler inlined as raw loads.
   This header reuses the existing concrete layouts so the offsets stay DB-anchored. */
#include "hkpShape.h"
#include "hkpShapeType.h"
#include "hkpConvexTransformShapeBase.h"  /* hkpSingleShapeContainer + hkpConvexTransformShapeBase (child hkpShape* @0x18) */

/* hkpTransformShape child-container view — hkpShape base (16) then the shared
   hkpSingleShapeContainer at offset 16, so the child hkpShape* sits at 0x14 (20).
   Matches DB types_members hkpTransformShape (disasm @0x83F976D8: lwz r29,0x14(r31)).
   Named distinctly from hkpTransformShapeLayout.h's hkpTransformShapeFull so this
   header can reuse hkpConvexTransformShapeBase.h's hkpSingleShapeContainer without
   the two colliding on that typedef. */
#include "hkTransform.h"
struct hkpTransformShapeChildView : hkpShape
{
    hkpSingleShapeContainer m_childShape; /* 16 -> child hkpShape* @0x14 */
    int m_childShapeSize;                 /* 24 */
    unsigned char _pad1C[4];              /* 28 */
    hkVector4 m_rotation;                 /* 32 — hkQuaternion (matches hkpTransformShapeFull) */
    hkTransform m_transform;              /* 48 (0x30) — the stored transform (TRANSFORM case) */
};
typedef struct hkpTransformShapeChildView hkpTransformShapeChildView;

/* hkpConvexTranslateShape — shares hkpConvexTransformShapeBase (size 32) then stores a
   BARE translation vector at 0x20 (no rotation; cut synthesizes an identity rotation for
   it). Disasm @0x83F977B4 lwz r29,0x18(r31) child; @0x83F977B8 addi r28,r31,0x20 then
   lvx128 of the 16-byte translation at shape+0x20. */
#include "hkVector4.h"
struct hkpConvexTranslateShape : hkpConvexTransformShapeBase
{
    hkVector4 m_translation; /* 32 (0x20) */
};
typedef struct hkpConvexTranslateShape hkpConvexTranslateShape;
