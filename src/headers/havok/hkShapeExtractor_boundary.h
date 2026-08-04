#pragma once
#include "hkArray.h"
#include "hkAabb.h"
#include "hkVector4.h"
#include "hkTransform.h"
#include "hkResult.h"
#include "hkBool.h"
#include "hkReferencedObject.h"

typedef struct hkpShape hkpShape;
typedef struct hkpConvexShape hkpConvexShape;
typedef struct hkpWorld hkpWorld;
typedef struct hkpCollisionDispatcher hkpCollisionDispatcher;

/* types_members hkdAabbIntersector (size 8) — just the hkReferencedObject base. */
struct hkdAabbIntersector : hkReferencedObject
{
};
typedef struct hkdAabbIntersector hkdAabbIntersector;

/* types_members hkdShapeExtractorUtil::Info (size 24); m_shapes holds the
   extracted shapes. */
typedef struct hkdShapeExtractorUtil_Info
{
    hkdAabbIntersector *m_volume;          /* 0x00 */
    int m_maxDepth;                        /* 0x04 */
    hkBool m_outside;                      /* 0x08 */
    unsigned char _pad09[3];               /* 0x09 */
    hkArray<const hkpShape *> m_shapes;    /* 0x0C */
} hkdShapeExtractorUtil_Info;

/* types_members hkdSphereExtractorVolume (size 32) — hkdAabbIntersector base,
   aligned sphere (centre xyz + radius w) at 0x10. */
struct hkdSphereExtractorVolume : hkdAabbIntersector
{
    unsigned char _pad08[8];  /* 0x08 — align m_sphere to 16 */
    hkVector4 m_sphere;       /* 0x10 */
};
typedef struct hkdSphereExtractorVolume hkdSphereExtractorVolume;

/* types_members hkpShapeConnectedCalculator (size 16) */
struct hkpShapeConnectedCalculator : hkReferencedObject
{
    hkpCollisionDispatcher *m_dispatcher;  /* 0x08 */
    float m_maxDistanceForConnection;      /* 0x0C */
};
typedef struct hkpShapeConnectedCalculator hkpShapeConnectedCalculator;

extern void *hkdSphereExtractorVolume_vftable; /* hkdSphereExtractorVolume::`vftable' */

void hkdShapeExtractorUtil_Info_construct(hkdShapeExtractorUtil_Info *info, hkdSphereExtractorVolume *volume, int maxDepth, int addReferences); /* Info::Info */
void hkdShapeExtractorUtil_Info_destruct(hkdShapeExtractorUtil_Info *info);
void hkdShapeExtractorUtil_extractRecurse(const hkAabb *aabb, const hkpShape *shape, hkdShapeExtractorUtil_Info *info, int depth);
void hkReferencedObject_addReferences(void *shapes, int count, int stride);
void hkReferencedObject_removeReferences(void *shapes, int count, int stride);
void hkArray_spliceInto(hkArray<> *self, int index, int numToRemove, void *insertData, int numToInsert); /* hkArray::spliceInto */
/* Returns hkResult (mangled ret type is AW4hkResult); success == HK_SUCCESS(0). */
hkResult hkpShapeCutterUtil_flattenIntoConvexShapes(const hkpShape *shape, const hkTransform *transform, hkArray<> *transformsOut, hkArray<> *convexShapesOut);
void hkpShapeConnectedCalculator_construct(hkpShapeConnectedCalculator *self, hkpCollisionDispatcher *dispatcher, float tolerance);
void hkpShapeCutterUtil_findConnectedIslands(hkpShapeConnectedCalculator *calc, const hkArray<> *convexShapes, const hkArray<> *transforms, hkArray<> *islandSizesOut, hkArray<> *islandMembersOut);
const hkpShape *hkpShapeCutterUtil_createCompound(const hkpConvexShape **shapes, const hkTransform *transforms, int count);
