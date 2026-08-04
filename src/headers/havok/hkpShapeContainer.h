#pragma once
#include "hkpShape.h"

/* hkpShapeContainer — the abstract child-iteration interface multiply-inherited
   by shape collections (hkpShapeCollection sub-object at +16 of the shape). It is
   a pure-virtual class: the concrete implementation lives in the derived shape's
   second vtable. Instances are only ever referenced through a pointer whose first
   word is the container vtable pointer, so the object body is opaque here.

   Vtable slot layout is taken from the dispatch sites in
   hkpConvexVerticesConnectivityUtil::ensureConnectivityAll (disasm 0x83F93930):
       lwz r10, 0x08(vtbl) ; getFirstKey   (@0x83F93968)
       lwz r10, 0x14(vtbl) ; getChildShape (@0x83F93990)
       lwz r10, 0x0C(vtbl) ; getNextKey    (@0x83F939B4)
   The remaining slots are named for layout completeness only. */

typedef struct hkpShapeContainer
{
    void *__vftable;                   /* 0 hkpShapeContainer::__vftable */
} hkpShapeContainer;

typedef struct hkpShapeContainer_vtbl
{
    void (*dtor)(hkpShapeContainer *self);                                   /* 0x00 */
    int (*getNumChildShapes)(const hkpShapeContainer *self);                 /* 0x04 */
    int (*getFirstKey)(const hkpShapeContainer *self);                       /* 0x08 */
    int (*getNextKey)(const hkpShapeContainer *self, int oldKey);            /* 0x0C */
    unsigned int (*getCollisionFilterInfo)(const hkpShapeContainer *self, int key); /* 0x10 */
    const hkpShape *(*getChildShape)(const hkpShapeContainer *self,
                                     int key, void *buffer);                 /* 0x14 */
} hkpShapeContainer_vtbl;
