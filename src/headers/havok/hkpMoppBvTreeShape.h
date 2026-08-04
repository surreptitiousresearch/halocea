#pragma once

/* hkpMoppBvTreeShape — minimal plain-C mirror of the DB layout (types_members
   hkpMoppBvTreeShape: baseclass hkMoppBvTreeShapeBase @0 (48 bytes), m_child
   hkpSingleShapeContainer @48, m_childSize int @56). Only the members consumed
   by re-sourced code are typed; the base stays opaque (Havok boundary). */

typedef struct hkpShape hkpShape; /* opaque */

/* hkpSingleShapeContainer (DB: hkpShapeContainer base vtable @0, m_childShape @4) */
typedef struct hkpSingleShapeContainer
{
    void *__vftable;                 /* 0  hkpShapeContainer */
    const hkpShape *m_childShape;    /* 4  */
} hkpSingleShapeContainer;

typedef struct hkpMoppBvTreeShape
{
    unsigned char _base[48];         /* 0  hkMoppBvTreeShapeBase (opaque boundary) */
    hkpSingleShapeContainer m_child; /* 48 */
    int m_childSize;                 /* 56 */
} hkpMoppBvTreeShape;
