#pragma once
/* Typed C++ layout of hkpTransformShape, used by its constructor/destructor TUs.
   (The sibling hkpTransformShape.h keeps an opaque 112-byte form for the pure-C
   connectivity TU that only stack-allocates and passes the buffer by pointer;
   this header is the field-typed view for the methods that populate it.) */
#include "hkpShape.h"
#include "hkVector4.h"
#include "hkTransform.h"

typedef struct hkpSingleShapeContainer_vtbl hkpSingleShapeContainer_vtbl; /* boundary vtable */

/* hkpSingleShapeContainer (8 bytes) — single-child shape holder. */
typedef struct hkpSingleShapeContainer
{
    hkpSingleShapeContainer_vtbl *__vftable; /* 0x00 */
    const hkpShape *m_childShape;            /* 0x04 */
} hkpSingleShapeContainer;

/* hkpTransformShape (112 bytes) — see DB types_members / hkpTransformShape.h. */
struct hkpTransformShapeFull : hkpShape
{
    hkpSingleShapeContainer m_childShape; /* 16 */
    int m_childShapeSize;                 /* 24 */
    unsigned char _pad1C[4];              /* 28 */
    hkVector4 m_rotation;                 /* 32 — hkQuaternion */
    hkTransform m_transform;              /* 48 */
};
typedef struct hkpTransformShapeFull hkpTransformShapeFull;
