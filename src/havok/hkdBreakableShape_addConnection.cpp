/* ?addConnection@hkdBreakableShape@@QAAXABUConnection@1@@Z @0x83849D68 */
#include "../headers/havok/hkdBreakableShape.h"
#include "../headers/havok/hkdBreakableShape_Connection.h"

extern void hkArrayUtil__reserveMore(void *array, int sizeElem); /* hkArrayUtil::_reserveMore */

/* hkdBreakableShape::addConnection — append a Connection to m_connections,
   growing the array when full.
   DEVIATION: the original copies the 64-byte record with VMX128 loads/stores
   plus scalar tail moves; reconstructed as a struct assignment. */
void hkdBreakableShape_addConnection(hkdBreakableShape *self, const hkdBreakableShape_Connection *connection)
{
    if (self->m_connections.m_size == (self->m_connections.m_capacityAndFlags & 0x3FFFFFFF))
        hkArrayUtil__reserveMore(&self->m_connections.m_data, 64); /* sizeof(Connection) */
    self->m_connections.m_data[self->m_connections.m_size++] = *connection;
}
