#include "../headers/havok/hkdGeometry.h"
#include "../headers/havok/hkdGeometry_Face.h"
#include "../headers/havok/hkdGeometry_Triangle.h"
#include "../headers/havok/hkdGeometry_HalfEdge.h"
#include "../headers/havok/hkArray.h"

/* Boundary callee (2-level descent) — grow a raw hkArray to hold newCapacity
   elements of sizeElem bytes. */
extern void hkArrayUtil__reserve(void *arrayData, int newCapacity, int sizeElem); /* hkArrayUtil::_reserve — arg0 void* per mangled ?_reserve@hkArrayUtil@@YAXPAXHH@Z */

/* hkdGeometry::calculateHalfEdges — enumerate every triangle of every face and
   emit its three directed half-edges into edgesOut. For triangle (i0,i1,i2) with
   per-edge neighbouring faces (n0,n1,n2) the edges are (i0->i1, n0), (i1->i2, n1)
   and (i2->i0, n2), all tagged with the owning face index. The output array is
   grown in place (doubling, or exactly to fit, whichever is larger) as needed. */
void hkdGeometry_calculateHalfEdges(hkdGeometry *self, hkArray<HalfEdge> *edgesOut)
{
    int faceIndex;
    int numFaces = self->m_faces.m_size;

    edgesOut->m_size = 0;

    for (faceIndex = 0; faceIndex < numFaces; ++faceIndex)
    {
        const Face *face = &self->m_faces.m_data[faceIndex];
        const Triangle *tri = &self->m_triangles.m_data[face->m_startTriangleIndex];
        int remaining = face->m_numTriangles;

        for (; remaining > 0; --remaining, ++tri)
        {
            int size = edgesOut->m_size;
            int capacity = edgesOut->m_capacityAndFlags & 0x3FFFFFFF;
            HalfEdge *out;

            if (capacity < size + 3)
            {
                int newCapacity = 2 * capacity;
                if (size + 3 >= newCapacity)
                    newCapacity = size + 3;
                hkArrayUtil__reserve(&edgesOut->m_data, newCapacity, 8);
            }

            out = &edgesOut->m_data[size];
            edgesOut->m_size = size + 3;

            out[0].m_a = tri->m_vertexIndices[0];
            out[0].m_b = tri->m_vertexIndices[1];
            out[0].m_face = (unsigned short)faceIndex;
            out[0].m_neighbouringFace = tri->m_neighbouringFaces[0];

            out[1].m_a = tri->m_vertexIndices[1];
            out[1].m_b = tri->m_vertexIndices[2];
            out[1].m_face = (unsigned short)faceIndex;
            out[1].m_neighbouringFace = tri->m_neighbouringFaces[1];

            out[2].m_a = tri->m_vertexIndices[2];
            out[2].m_b = tri->m_vertexIndices[0];
            out[2].m_face = (unsigned short)faceIndex;
            out[2].m_neighbouringFace = tri->m_neighbouringFaces[2];
        }
    }
}
