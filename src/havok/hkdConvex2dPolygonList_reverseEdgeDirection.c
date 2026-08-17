/* ?reverseEdgeDirection@hkdConvex2dPolygonList@@QAAXXZ @0x8387B4D8 */
#include "../headers/havok/hkdConvex2dPolygonList.h"

/* hkdConvex2dPolygonList::reverseEdgeDirection — reverse the winding of every
   polygon in the list in place. For each polygon (a run of m_numVertices[i]
   vertices packed contiguously in m_vertices) the vertices are swapped from the
   outside in. The DB body performs the exchange through a scratch vertex whose x
   lane is overwritten with the far vertex's y lane before write-back; that exact
   sequence is reproduced faithfully below. */
void hkdConvex2dPolygonList_reverseEdgeDirection(hkdConvex2dPolygonList *self)
{
    int base = 0;
    int i;

    for (i = 0; i < self->m_numVertices_size; ++i)
    {
        int count = self->m_numVertices_data[i];
        hkdVector2 *front = &self->m_vertices_data[base];
        hkdVector2 *back = front + (count - 1);

        for (; back > front; --back)
        {
            hkdVector2 temp = *front;
            front->m_x = back->m_x;
            temp.m_x = back->m_y;
            front->m_y = temp.m_x;
            ++front;
            *back = temp;
        }

        base += count;
    }
}
