#pragma once
#include "hkReferencedObject.h"
#include "hkBool.h"
#include "hkEnum.h"
#include "hkRefPtr.h"

/* size 28. Layout from DB type hkdFracture. The DB nests the enums inside hkdFracture
   (hkdFracture::Type, hkdFracture::RefitPhysicsType, hkdFracture::ConnectivityType); they are
   scoped in a namespace here to keep the unqualified names collision-free across headers. */
namespace hkdFractureEnums
{
    /* types_enum_values hkdFracture::Type */
    enum Type
    {
        TYPE_INVALID     = 0,
        TYPE_SPLITINHALF = 1,
        TYPE_WOOD        = 2,
        TYPE_RANDOMSPLIT = 3,
        TYPE_SLICE       = 4,
        TYPE_PIE         = 5,
        TYPE_VORONOI     = 6,
        TYPE_DEBRIS      = 7,
        TYPE_NUM_TYPES   = 8,
    };

    /* types_enum_values hkdFracture::RefitPhysicsType */
    enum RefitPhysicsType
    {
        REFIT_CONVEX_HULL           = 0,
        REFIT_SHRINK_TO_CONVEX_HULL = 1,
    };

    /* types_enum_values hkdFracture::ConnectivityType */
    enum ConnectivityType
    {
        CONNECTIVITY_INHERITED = 0,
        CONNECTIVITY_PARTIAL   = 1,
        CONNECTIVITY_FULL      = 2,
        CONNECTIVITY_NONE      = 3,
    };
}

typedef struct hkdFracture
{
    hkReferencedObject base;                                 /* 0 */
    float m_rootToLeafRatio;                                 /* 8 */
    float m_minimumSize;                                     /* 12 */
    float m_maxDistanceForConnection;                        /* 16 */
    hkEnum<hkdFractureEnums::Type,unsigned char> m_type;     /* 20 */
    hkEnum<hkdFractureEnums::RefitPhysicsType,unsigned char> m_refitPhysicsShapes; /* 21 */
    hkEnum<hkdFractureEnums::ConnectivityType,unsigned char> m_connectivityType;   /* 22 */
    hkBool m_flattenHierarchy;                               /* 23 */
    hkRefPtr<hkdFracture> m_childFracture;                   /* 24 */
} hkdFracture; /* 28 bytes */
