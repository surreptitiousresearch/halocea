#pragma once

/* hkpSingleShapeContainer (size 8, DB types_members) — an hkpShapeContainer that
   holds exactly one child shape: a vtable pointer plus the child pointer. Canonical
   home; previously restated in hkpConvexTransformShapeBase.h, hkpMoppBvTreeShape.h
   and hkpTransformShapeLayout.h, which left the bulk header probe with an ODR
   redefinition and the record unmeasurable. */
struct hkpShape;

typedef struct hkpSingleShapeContainer
{
    void *__vftable;                    /* 0 hkpSingleShapeContainer::`vftable' */
    const struct hkpShape *m_childShape;/* 4 */
} hkpSingleShapeContainer;
