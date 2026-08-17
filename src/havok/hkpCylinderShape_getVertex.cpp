/* ?getVertex@hkpCylinderShape@@QBAABVhkVector4@@H@Z @0x83604918 */
#include "../headers/havok/hkpCylinderShape.h"
#include "../headers/havok/hkVector4.h"

/* hkpCylinderShape::getVertex — the i-th cap centre (0 = m_vertexA, 1 = m_vertexB).
   The two endpoints are consecutive hkVector4s, so indexing off m_vertexA reaches
   either one. */
const hkVector4 *hkpCylinderShape_getVertex(const hkpCylinderShape *self, int i)
{
    return &(&self->m_vertexA)[i];
}
