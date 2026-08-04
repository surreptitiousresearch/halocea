#include "../headers/havok/hkMeshShape.h"

/* hkMeshShape is an abstract interface; getNumSections is pure virtual
   (vtable slot 0x0C), implemented by concrete shapes such as
   hkMemoryMeshShape / hkDefaultCompoundMeshShape. This wrapper is the virtual
   dispatch the frontier callers (matchGraphicsToGeometry,
   hkdMeshSplittingPlaneUtil_buildMeshSections) reach through the extern. */
typedef struct hkMeshShape_vtbl
{
    void (*dtor)(hkMeshShape *self);                    /* 0x00 */
    const void *(*getClassType)(hkMeshShape *self);     /* 0x04 */
    void (*calcContentStatistics)(hkMeshShape *self, void *, const void *); /* 0x08 */
    int (*getNumSections)(hkMeshShape *self);           /* 0x0C */
} hkMeshShape_vtbl;

int hkMeshShape_getNumSections(const hkMeshShape *self)
{
    hkMeshShape *s = (hkMeshShape *)self;
    return ((hkMeshShape_vtbl *)s->__vftable)->getNumSections(s);
}
