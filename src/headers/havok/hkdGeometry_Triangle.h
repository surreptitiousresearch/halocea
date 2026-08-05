#pragma once
#include <stdint.h>

/* hkdGeometry::Triangle, size 12 (DB types_members hkdGeometry::Triangle). */
typedef struct hkdGeometry_Triangle
{
    uint16_t m_vertexIndices[3];     /* 0 */
    uint16_t m_neighbouringFaces[3]; /* 6 */
} hkdGeometry_Triangle;

/* The DB spells this nested type hkdGeometry::Triangle; member declarations elsewhere
   reference the unqualified nested name. */
typedef hkdGeometry_Triangle Triangle;
