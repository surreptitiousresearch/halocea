#pragma once
#include "hkpConvexShape.h"

/* hkpSingleShapeContainer (size 8, DB types_members) — an hkpShapeContainer that
   holds exactly one child shape: a vtable pointer plus the child pointer. */
typedef struct hkpSingleShapeContainer
{
    void *__vftable;             /* 0 hkpSingleShapeContainer::`vftable' */
    const hkpShape *m_childShape;/* 4 */
} hkpSingleShapeContainer;

/* hkpConvexTransformShapeBase (size 32, DB types_members) — hkpConvexShape base
   (shape type/userData/refcount + radius) then the single-shape container and its
   cached size. Shared base of hkpConvexTransformShape / hkpConvexTranslateShape. */
struct hkpConvexTransformShapeBase : hkpConvexShape
{
    hkpSingleShapeContainer m_childShape; /* 20 */
    int m_childShapeSize;                 /* 28 */
};
typedef struct hkpConvexTransformShapeBase hkpConvexTransformShapeBase;

/* boundary data */
extern void *hkpConvexTransformShapeBase_vftable[];  /* hkpConvexTransformShapeBase::`vftable' */
extern void *hkpSingleShapeContainer_vftable[];      /* hkpSingleShapeContainer::`vftable' */

/* Correct ABI (mangled ??0hkpConvexTransformShapeBase@@...@W4hkpShapeType@@ M
   PBVhkpConvexShape@@ W4ReferencePolicy@hkpShapeContainer@@ @Z): NO transform
   argument — the transform is stored by the derived constructor. */
void hkpConvexTransformShapeBase_construct(hkpConvexTransformShapeBase *self, int shapeType,
                                           float radius, const hkpShape *childShape,
                                           int referencePolicy); /* hkpConvexTransformShapeBase::hkpConvexTransformShapeBase */
