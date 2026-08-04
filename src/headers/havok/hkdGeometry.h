#pragma once
#include "hkReferencedObject.h"
#include "hkArray.h"
#include "hkVector4.h"
#include "hkdGeometry_Face.h"
#include "hkdGeometry_Triangle.h"
#include "hkdGeometry_ObjectIdentifier.h"

/* size 60. Element types are the DB's unqualified nested names (hkdGeometry::Face, ...). */
typedef struct hkdGeometry
{
    hkReferencedObject base;             /* 0 */
    hkArray<Face> m_faces;               /* 8  */
    hkArray<Triangle> m_triangles;       /* 20 */
    hkArray<hkVector4> m_vertices;       /* 32 */
    hkArray<ObjectIdentifier> m_objectIds; /* 44 */
    struct hkdGeometry *m_parent;        /* 56 */
} hkdGeometry;
