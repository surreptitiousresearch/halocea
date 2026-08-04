#include "../headers/havok/hkpShapeCutterUtil_boundary.h"

/* Compute the outward plane equation of the face given by `idx` (positions from
   `vertices`) into `planeOut`. Outside this slice. */
extern void hkpShapeCutterUtil__calculatePlane(const int *idx, const hkArray<hkVector4> *vertices, hkVector4 *planeOut); /* hkpShapeCutterUtil::_calculatePlane */

/* Append one unsigned-short vertex index, growing the array when full. */
static void pushIndex(hkArray<unsigned short> *arr, unsigned short value)
{
    if (arr->m_size == (arr->m_capacityAndFlags & 0x3FFFFFFF))
        hkArrayUtil__reserveMore(&arr->m_data, 2);
    arr->m_data[arr->m_size++] = value;
}

/* hkpShapeCutterUtil::_addTriangle @ 0x83F94E70
   Emit a triangle face: append its plane equation to `planeEquationsOut`, its 3
   vertex indices to the connectivity index array, and a face vertex-count of 3. */
void hkpShapeCutterUtil__addTriangle(const int *idx, const hkpConvexVerticesConnectivity *connectivity,
                                     const hkArray<hkVector4> *vertices, hkArray<hkVector4> *planeEquationsOut)
{
    hkpConvexVerticesConnectivity *conn = (hkpConvexVerticesConnectivity *)connectivity;
    hkVector4 plane;
    int planeSize;

    hkpShapeCutterUtil__calculatePlane(idx, vertices, &plane);

    if (planeEquationsOut->m_size == (planeEquationsOut->m_capacityAndFlags & 0x3FFFFFFF))
        hkArrayUtil__reserveMore(&planeEquationsOut->m_data, 16);
    planeSize = planeEquationsOut->m_size;
    planeEquationsOut->m_data[planeSize] = plane;
    planeEquationsOut->m_size = planeSize + 1;

    pushIndex(&conn->m_vertexIndices, (unsigned short)idx[0]);
    pushIndex(&conn->m_vertexIndices, (unsigned short)idx[1]);
    pushIndex(&conn->m_vertexIndices, (unsigned short)idx[2]);

    if (conn->m_numVerticesPerFace.m_size == (conn->m_numVerticesPerFace.m_capacityAndFlags & 0x3FFFFFFF))
        hkArrayUtil__reserveMore(&conn->m_numVerticesPerFace.m_data, 1);
    conn->m_numVerticesPerFace.m_data[conn->m_numVerticesPerFace.m_size++] = 3;
}
