/* SphereGun_getGeometry @0x8385E858 */
#include "../headers/havok/hkdBreakableShape.h"
#include "../headers/havok/hkdGeometry.h"
#include "../headers/havok/hkReferencedObject.h"

extern const hkdGeometry *SphereGun_createCompoundGeometry(const hkdBreakableShape *shape);

/* Return the shape's geometry. A compound shape (type 2) synthesises one; a leaf
   shape returns its stored geometry with a fresh reference added. */
const hkdGeometry *SphereGun_getGeometry(const hkdBreakableShape *shape)
{
    const hkdGeometry *geometry;

    if (shape->m_type.m_storage == SHAPE_TYPE_COMPOUND) /* hkEnum<ShapeType>::m_storage */
        return SphereGun_createCompoundGeometry(shape);

    geometry = shape->m_geometry.m_pntr; /* hkRefPtr::m_pntr */
    if (geometry)
        hkReferencedObject_addReference((hkReferencedObject *)&geometry->base);
    return geometry;
}
