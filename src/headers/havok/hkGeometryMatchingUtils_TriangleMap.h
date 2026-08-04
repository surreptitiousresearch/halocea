#pragma once
/* External/SDK extern boundary — partial by design, see .complete/ESCALATIONS.md */

/* hkGeometryMatchingUtils::TriangleMap (size 1624).
   Per-geometry mapping produced by
   hkdGeometry::setFaceIdentifiersToBestMatchingTriangle: for each triangle of a
   geometry it records which reference triangle it matched.
   Both members are hkInplaceArray<T,N> (an hkArray backed by inline storage;
   m_data initially points at m_storage and m_capacityAndFlags carries N together
   with the DONT_DEALLOCATE flag 0x80000000). */

/* hkGeometryMatchingUtils::TriangleMap::Hit (size 12) */
typedef struct hkGeometryMatchingUtils_TriangleMap_Hit
{
    unsigned short m_geometryIndex;   /* 0 */
    unsigned char m_indexOffset;      /* 2 */
    /* 1 byte pad */
    int m_triangleIndex;              /* 4 */
    bool m_flippedWinding;            /* 8 */
    /* 3 bytes pad -> 12 */
} hkGeometryMatchingUtils_TriangleMap_Hit;

/* hkInplaceArray<unsigned int,16> (size 76) */
typedef struct hkInplaceArray_uint_16
{
    unsigned int *m_data;                 /* 0  */
    int m_size;                           /* 4  */
    int m_capacityAndFlags;               /* 8  */
    unsigned int m_storage[16];           /* 12 */
} hkInplaceArray_uint_16;

/* hkInplaceArray<hkGeometryMatchingUtils::TriangleMap::Hit,128> (size 1548) */
typedef struct hkInplaceArray_Hit_128
{
    hkGeometryMatchingUtils_TriangleMap_Hit *m_data;      /* 0  */
    int m_size;                                           /* 4  */
    int m_capacityAndFlags;                               /* 8  */
    hkGeometryMatchingUtils_TriangleMap_Hit m_storage[128];/* 12 */
} hkInplaceArray_Hit_128;

typedef struct hkGeometryMatchingUtils_TriangleMap
{
    hkInplaceArray_uint_16 m_startIndexPerGeometry;   /* 0  */
    hkInplaceArray_Hit_128 m_foundReferenceTriangle;  /* 76 */
} hkGeometryMatchingUtils_TriangleMap;

/* The DB spells this nested type hkGeometryMatchingUtils::TriangleMap; member declarations
   elsewhere reference the unqualified nested name. */
typedef hkGeometryMatchingUtils_TriangleMap TriangleMap;
