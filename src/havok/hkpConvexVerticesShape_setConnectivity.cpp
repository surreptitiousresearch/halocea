#include "../headers/havok/hkpConvexVerticesShape.h"
#include "../headers/havok/hkReferencedObject.h"

/* hkpConvexVerticesShape::sortPlanes — reorders plane equations to match the new
   connectivity; outside this slice. */
extern void hkpConvexVerticesShape_sortPlanes(hkpConvexVerticesShape *self); /* hkpConvexVerticesShape::sortPlanes */

/* hkpConvexVerticesShape::setConnectivity @ 0x83610D20
   Swap the shape's connectivity object (with refcount handover) and, when a new
   one is attached, re-sort the plane equations to match it. */
void hkpConvexVerticesShape_setConnectivity(hkpConvexVerticesShape *self,
                                            const hkpConvexVerticesConnectivity *connect)
{
    const hkpConvexVerticesConnectivity *old;

    if (connect)
        hkReferencedObject_addReference((hkReferencedObject *)&connect->base);

    old = self->m_connectivity;
    if (old)
        hkReferencedObject_removeReference((hkReferencedObject *)&old->base);

    self->m_connectivity = connect;
    if (connect)
        hkpConvexVerticesShape_sortPlanes(self);
}
