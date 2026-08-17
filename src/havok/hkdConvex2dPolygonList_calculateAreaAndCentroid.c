/* ?calculateAreaAndCentroid@hkdConvex2dPolygonList@@QBAMAAUhkdVector2@@@Z @0x8387B558 */
#include "../headers/havok/hkdConvex2dPolygonList.h"

/* hkdConvex2dPolygonList::calculateAreaAndCentroid — total signed area and
   area-weighted centroid of every polygon in the list, aggregated. Each polygon
   is fan-triangulated from its first vertex; for each triangle (v0, vi, vi+1) the
   signed cross product doubles as twice its area, and the running centroid
   accumulates (v0+vi+vi+1) * cross. The centroid is written to centroidOut and
   the total area (half the summed cross products) is returned.

   DEVIATION: the DB body unrolls the per-triangle FMA accumulation two-wide over
   VMX lanes; reproduced here as the equivalent scalar shoelace sum. */
float hkdConvex2dPolygonList_calculateAreaAndCentroid(hkdConvex2dPolygonList *self,
                                                      hkdVector2 *centroidOut)
{
    int numPolys = self->m_numVertices_size;
    float crossSum = 0.0f;   /* = 2 * total area */
    float momentX = 0.0f;
    float momentY = 0.0f;
    int base = 0;
    int p;

    for (p = 0; p < numPolys; ++p)
    {
        int count = self->m_numVertices_data[p];
        const hkdVector2 *v = &self->m_vertices_data[base];
        int i;

        for (i = 1; i + 1 < count; ++i)
        {
            float ax = v[i].m_x - v[0].m_x;
            float ay = v[i].m_y - v[0].m_y;
            float bx = v[i + 1].m_x - v[0].m_x;
            float by = v[i + 1].m_y - v[0].m_y;
            float cross = ax * by - bx * ay;

            crossSum += cross;
            momentX += (v[0].m_x + v[i].m_x + v[i + 1].m_x) * cross;
            momentY += (v[0].m_y + v[i].m_y + v[i + 1].m_y) * cross;
        }

        base += count;
    }

    centroidOut->m_x = momentX * (1.0f / (crossSum * 3.0f));
    centroidOut->m_y = momentY * (1.0f / (crossSum * 3.0f));
    return crossSum * 0.5f;
}
