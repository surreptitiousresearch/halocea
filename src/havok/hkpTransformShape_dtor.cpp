// FUNCTION_INDEX entry: hkpTransformShape_dtor @0x835FD8D0 (??1hkpTransformShape@@UAA@XZ)
#include "../headers/havok/hkpTransformShape.h"
#include "../headers/havok/hkpTransformShapeLayout.h"

/* Vtables re-installed while unwinding the object (boundary data). */
extern void *hkpSingleShapeContainer_vftable[]; /* hkpSingleShapeContainer::`vftable' */
extern void *hkpShapeContainer_vftable[];       /* hkpShapeContainer::`vftable' */
extern void *hkBaseObject_vftable[];            /* hkBaseObject::`vftable' */

extern void hkReferencedObject_removeReference(hkReferencedObject *self); /* hkReferencedObject::removeReference */

/* hkpTransformShape::~hkpTransformShape — drop the reference held on the child
   shape and walk the vtables back down the container/base hierarchy (matching the
   compiler-generated destructor epilogue). */
extern "C" void hkpTransformShape_dtor(hkpTransformShape *selfOpaque)
{
    hkpTransformShapeFull *self = (hkpTransformShapeFull *)selfOpaque;
    const hkpShape *child = self->m_childShape.m_childShape;

    self->m_childShape.__vftable = (hkpSingleShapeContainer_vtbl *)hkpSingleShapeContainer_vftable;
    if (child)
        hkReferencedObject_removeReference((hkReferencedObject *)child);

    self->m_childShape.__vftable = (hkpSingleShapeContainer_vtbl *)hkpShapeContainer_vftable;
    self->base.__vftable = (hkBaseObject_vtbl *)hkBaseObject_vftable;
}
