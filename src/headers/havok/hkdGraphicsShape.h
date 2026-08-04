#pragma once
#include "hkReferencedObject.h"

/* The DB nests this enum inside hkdGraphicsShape (hkdGraphicsShape::Type); scoped in a
   namespace here to keep the unqualified name collision-free across headers. */
namespace hkdGraphicsShapeEnums
{
    /* types_enum_values hkdGraphicsShape::Type */
    enum Type
    {
        TYPE_SINGLE     = 0,
        TYPE_COMPOUND   = 1,
        TYPE_DEFORMABLE = 2,
    };
}

/* size 16. Layout from DB type hkdGraphicsShape. */
typedef struct hkdGraphicsShape
{
    hkReferencedObject base;             /* 0 */
    hkdGraphicsShapeEnums::Type m_type;  /* 8 — DB hkdGraphicsShape::Type */
    int m_numChildTransforms;            /* 12 */
} hkdGraphicsShape;
