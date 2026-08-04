#pragma once

/* hkdGeometry::HalfEdge (size 8, DB types_members) — one directed edge of a face
   used by the consistency pass. m_a/m_b are the two vertex indices, m_face the
   owning face, m_neighbouringFace the face on the far side (0xFFFF = boundary). */
typedef struct hkdGeometry_HalfEdge
{
    unsigned short m_a;                 /* 0 */
    unsigned short m_b;                 /* 2 */
    unsigned short m_face;              /* 4 */
    unsigned short m_neighbouringFace;  /* 6 */
} hkdGeometry_HalfEdge;

/* The DB spells this nested type hkdGeometry::HalfEdge; member declarations
   elsewhere reference the unqualified nested name. */
typedef hkdGeometry_HalfEdge HalfEdge;
