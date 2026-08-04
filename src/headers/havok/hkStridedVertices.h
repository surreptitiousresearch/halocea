#pragma once

/* hkStridedVertices (size 12, DB types_members) — a strided view over a raw
   float vertex buffer handed to convex-hull construction. */
typedef struct hkStridedVertices
{
    const float *m_vertices;  /* 0 */
    int m_numVertices;        /* 4 */
    int m_striding;           /* 8 — bytes between consecutive vertices */
} hkStridedVertices;
