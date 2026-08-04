#pragma once

/* hkdGeometry::Triangle, size 12 (DB types_members hkdGeometry::Triangle). */
typedef struct hkdGeometry_Triangle
{
    unsigned __int16 m_vertexIndices[3];     /* 0 */
    unsigned __int16 m_neighbouringFaces[3]; /* 6 */
} hkdGeometry_Triangle;

/* The DB spells this nested type hkdGeometry::Triangle; member declarations elsewhere
   reference the unqualified nested name. */
typedef hkdGeometry_Triangle Triangle;
