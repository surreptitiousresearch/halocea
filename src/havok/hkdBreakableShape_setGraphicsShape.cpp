#include "../headers/havok/hkdBreakableShape.h"
#include "../headers/havok/hkdGraphicsShape.h"
#include "../headers/havok/hkReferencedObject.h"

/* hkdBreakableShape::setGraphicsShape — swap the referenced graphics shape,
   maintaining reference counts (add on the new, remove on the old). */
void hkdBreakableShape_setGraphicsShape(hkdBreakableShape *self, const hkdGraphicsShape *shape)
{
    hkdGraphicsShape *old;
    if (shape)
        hkReferencedObject_addReference((hkReferencedObject *)shape);
    old = (hkdGraphicsShape *)self->m_graphicsShape.m_pntr;
    if (old)
        hkReferencedObject_removeReference((hkReferencedObject *)old);
    self->m_graphicsShape.m_pntr = shape;
}
