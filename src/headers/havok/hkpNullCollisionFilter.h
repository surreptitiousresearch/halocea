#pragma once
#include "hkReferencedObject.h"
#include "hkEnum.h"

/* hkpCollisionFilter / hkpNullCollisionFilter (size 48, DB types_members).
   hkpCollisionFilter multiply-inherits five filter interfaces; in memory that is
   the hkReferencedObject sub-object (vtable + refcount) at 0 followed by four more
   bare interface vtable pointers at 8/12/16/20, padding, then the filter-type
   enum. hkpNullCollisionFilter adds no data — it only overrides the interface
   methods (every isCollisionEnabled returns true) and tags m_type. */
typedef struct hkpCollisionFilter
{
    hkReferencedObject base;                                 /* 0  vtable(hkReferencedObject) + refcount */
    void *m_collidableCollidableFilter_vftable;              /* 8  */
    void *m_shapeCollectionFilter_vftable;                   /* 12 */
    void *m_rayShapeCollectionFilter_vftable;                /* 16 */
    void *m_rayCollidableFilter_vftable;                     /* 20 */
    unsigned int m_prepad[2];                                /* 24 */
    hkEnum<int,unsigned int> m_type;                         /* 32 hkEnum<hkpFilterType,unsigned int> */
    unsigned int m_postpad[3];                               /* 36 */
} hkpCollisionFilter;

typedef hkpCollisionFilter hkpNullCollisionFilter;

/* hkpCollisionFilter::hkpFilterType — only HK_FILTER_NULL (1) is written here. */
#define HK_FILTER_NULL 1

/* boundary — base constructor (level-1 callee). */
extern void hkpCollisionFilter_construct(hkpCollisionFilter *self); /* hkpCollisionFilter::hkpCollisionFilter */

/* boundary vtable symbols. The null filter installs its own five interface
   vtables; the destructor restores each interface's own base vtable. */
extern void *hkpNullCollisionFilter_vftable_hkReferencedObject;
extern void *hkpNullCollisionFilter_vftable_hkpCollidableCollidableFilter;
extern void *hkpNullCollisionFilter_vftable_hkpRayShapeCollectionFilter;
extern void *hkpNullCollisionFilter_vftable_hkpShapeCollectionFilter;
extern void *hkpNullCollisionFilter_vftable_hkpRayCollidableFilter;
extern void *hkBaseObject_vftable;
extern void *hkpCollidableCollidableFilter_vftable;
extern void *hkpShapeCollectionFilter_vftable;
extern void *hkpRayShapeCollectionFilter_vftable;
extern void *hkpRayCollidableFilter_vftable;

void hkpNullCollisionFilter_construct(hkpNullCollisionFilter *self); /* hkpNullCollisionFilter::hkpNullCollisionFilter */
void hkpNullCollisionFilter_destruct(hkpNullCollisionFilter *self);  /* hkpNullCollisionFilter::~hkpNullCollisionFilter */
