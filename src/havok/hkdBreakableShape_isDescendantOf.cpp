#include "../headers/havok/hkdBreakableShape.h"

/* hkdBreakableShape::isDescendantOf — walk the parent chain upward from this
   shape; return 1 if parentShape is an ancestor, 0 otherwise (and 0 when called
   on a null shape). */
int hkdBreakableShape_isDescendantOf(const hkdBreakableShape *self, const hkdBreakableShape *parentShape)
{
    const hkdBreakableShape *current = self;

    if (!self)
        return 0;

    do
    {
        current = current->m_parent;
        if (current == parentShape)
            return 1;
    } while (current);

    return 0;
}
