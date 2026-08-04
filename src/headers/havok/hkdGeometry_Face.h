#pragma once

/* hkdGeometry::FaceIdentifier, size 8 */
typedef struct hkdGeometry_FaceIdentifier
{
    unsigned short m_userData[4]; /* 0 */
} hkdGeometry_FaceIdentifier;

/* The DB spells this nested type hkdGeometry::FaceIdentifier; member declarations
   reference the unqualified nested name. */
typedef hkdGeometry_FaceIdentifier FaceIdentifier;

/* hkdGeometry::Face, size 16 */
typedef struct hkdGeometry_Face
{
    FaceIdentifier m_support;             /* 0 — DB hkdGeometry::FaceIdentifier */
    unsigned short m_startTriangleIndex;  /* 8 */
    unsigned short m_numTriangles;        /* 10 */
    unsigned short m_objectIndex;         /* 12 */
    unsigned short m_parentFaceIndex;     /* 14 */
} hkdGeometry_Face;

/* The DB spells this nested type hkdGeometry::Face; member declarations elsewhere
   reference the unqualified nested name. */
typedef hkdGeometry_Face Face;
