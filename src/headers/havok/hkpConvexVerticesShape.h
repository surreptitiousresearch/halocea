#pragma once
#include "hkpConvexShape.h"
#include "hkVector4.h"
#include "hkArray.h"
#include "hkReferencedObject.h"

/* hkpConvexVerticesConnectivity (size 32, DB types_members) — per-hull vertex
   adjacency generated on demand by the connectivity util. */
typedef struct hkpConvexVerticesConnectivity
{
    hkReferencedObject base;                     /* 0  */
    hkArray<unsigned short> m_vertexIndices;     /* 8  */
    hkArray<unsigned char>  m_numVerticesPerFace;/* 20 */
} hkpConvexVerticesConnectivity;

/* hkpConvexVerticesShape (size 96, DB types_members) — convex hull described by
   its rotated vertices and plane equations, with an optional connectivity object.
   inherited m_type/m_radius are touched by the connectivity pass. */

/* hkpConvexVerticesShape::FourVectors (size 48) — a SoA block of up to four
   vertices: m_x holds the four x lanes, m_y the y lanes, m_z the z lanes. The
   packed store is addressed as a flat float array base[12*(i/4) + (i&3)] for x
   (and +4 / +8 for y / z). */
typedef struct hkpConvexVerticesShape_FourVectors
{
    hkVector4 m_x; /* 0x00 */
    hkVector4 m_y; /* 0x10 */
    hkVector4 m_z; /* 0x20 */
} hkpConvexVerticesShape_FourVectors;

struct hkpConvexVerticesShape : hkpConvexShape
{
    unsigned char _pad14[12];                    /* 20  — pad up to m_aabbHalfExtents */
    hkVector4 m_aabbHalfExtents;                 /* 32  */
    hkVector4 m_aabbCenter;                      /* 48  */
    hkArray<hkpConvexVerticesShape_FourVectors> m_rotatedVertices; /* 64  */
    int m_numVertices;                           /* 76  */
    hkArray<hkVector4> m_planeEquations;         /* 80  */
    const hkpConvexVerticesConnectivity *m_connectivity; /* 92 */
};
typedef struct hkpConvexVerticesShape hkpConvexVerticesShape;

/* Boundary callees for the connectivity pass (2-level descent). */
extern hkpConvexVerticesConnectivity *hkpConvexVerticesConnectivityUtil_findConnectivity(hkpConvexVerticesShape *shape); /* hkpConvexVerticesConnectivityUtil::findConnectivity — non-const return per DB prototype */
extern void hkpConvexVerticesShape_setConnectivity(hkpConvexVerticesShape *shape, const hkpConvexVerticesConnectivity *connectivity); /* hkpConvexVerticesShape::setConnectivity */

/* Vertex/plane accessors used by transformVerticesAndPlaneEquations and cut. */
extern void hkpConvexVerticesShape_getOriginalVertices(hkpConvexVerticesShape *shape, hkArray<hkVector4> *verticesOut); /* hkpConvexVerticesShape::getOriginalVertices */
extern const hkArray<hkVector4> *hkpConvexVerticesShape_getPlaneEquations(hkpConvexVerticesShape *shape);               /* hkpConvexVerticesShape::getPlaneEquations */
extern void hkpConvexVerticesShape_copyVertexData(hkpConvexVerticesShape *shape, const float *vertexIn, int byteStriding, int numVertices); /* hkpConvexVerticesShape::copyVertexData */

/* DEVIATION: at the call site this is a virtual dispatch through the shape vtable
   (getNumCollisionSpheresImpl slot); wrapped here as a boundary accessor that
   returns the packed vertex-block count. */
extern int hkpConvexVerticesShape_getNumCollisionSpheresImpl(hkpConvexVerticesShape *shape); /* hkpConvexVerticesShape::getNumCollisionSpheresImpl */

/* In-place constructor and volume helper used by the cut path. */
extern void hkpConvexVerticesShape_construct(hkpConvexVerticesShape *self, const struct hkStridedVertices *vertices, const hkArray<hkVector4> *planeEquations, float radius); /* hkpConvexVerticesShape::hkpConvexVerticesShape */
extern float hkpConvexVerticesConnectivityUtil_calculateVolume(const hkArray<hkVector4> *vertices, const hkpConvexVerticesConnectivity *connectivity); /* hkpConvexVerticesConnectivityUtil::calculateVolume */
