#pragma once
/* hkpTransformShape (112 bytes) — DB types_members:
     hkpShape base                 [0..16)
     hkpSingleShapeContainer m_childShape [16..24)
     int  m_childShapeSize         [24]
     hkQuaternion m_rotation       [32..48)
     hkTransform  m_transform      [48..112)
   The calculator only stack-constructs and destructs these buffers and passes
   them to hkpShapeCutterUtil::cut, so everything past the base stays opaque here.
   The hkpShape base IS spelled out, though: `cut` takes an hkpShape *, and in C
   that upcast has to be a real member access rather than an implicit conversion. */
#include "hkpShape.h"

typedef struct hkpTransformShape
{
    hkpShape      base;             /*  0..16  — DB: hkpShape base            */
    unsigned char m_opaque[96];     /* 16..112 — m_childShape, m_childShapeSize,
                                                 m_rotation, m_transform      */
} hkpTransformShape;
