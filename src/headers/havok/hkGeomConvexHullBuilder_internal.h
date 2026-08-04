#pragma once
#include "hkVector4.h"
#include "hkArray.h"
#include "hkBool.h"
#include "hkGeomHull.h"
#include "hkGeomConvexHull.h"
#include "hkMatrix3.h"
#include "hkAabb.h"
#include "hkThreadMemory.h"

/* Internal prototypes for the hkGeomConvexHullBuilder / hkGeomConvexHullTester
   cluster. The functions in THIS wave (declared here and defined in src/havok) plus
   the sibling builder helpers that remain boundary externs (the next frontier). */

/* ---- reconstructed in this wave ---- */
typedef hkBool *(*hkVector4Less)(hkBool *result, hkVector4 *lhs, hkVector4 *rhs);

void hkAlgorithm_quickSortRecursive_hkVector4(hkVector4 *arr, int d, int h, hkVector4Less cmpLess);
hkBool *hkGeomConvexHullBuilder_vectorLessAndMergeCoordinates(hkBool *result, hkVector4 *a, hkVector4 *b);
void hkGeomConvexHullBuilder_weldXsortedVertices(float weldTolerance, hkArray<hkVector4> *verts, int *numUniqueOut);
void hkGeomConvexHullBuilder_removeCollinearVertices(hkArray<hkVector4> *vertices, float degenerateTolerance);
void hkGeomConvexHullBuilder_generateHullFromPlanarPoints(const hkVector4 *planeEqn, const hkVector4 *verticesIn,
                                                          int numVerts, hkArray<hkVector4> *usedVertices,
                                                          hkArray<hkVector4> *planeEquationsOut);
hkBool *hkGeomConvexHullBuilder_buildPlaneEquations(hkBool *result, const hkGeomConvexHullTolerances *tolerances,
        hkGeomHull *hull, const hkArray<hkVector4> *usedVertices, hkVector4 *planarPlaneEquationOut,
        hkBool *isPlanarOut, hkArray<hkVector4> *planeEquationsOut,
        hkArray<hkGeomConvexHullBuilder_PlaneAndPoints> *tangentPlanesOut);
void hkGeomConvexHullBuilder_buildConvexHull(const hkGeomConvexHullTolerances *tolerances, const hkVector4 *verts,
        int numVertices, hkGeomHull *hullOut, hkArray<hkVector4> *usedVerticesOut);

/* ---- reconstructed in THIS wave (saber7) ---- */
void hkGeomConvexHullBuilder_convertToUnitCube(hkArray<hkVector4> *verts, hkVector4 *scaleOut, hkVector4 *offsetOut);
void hkGeomConvexHullBuilder_convertFromUnitCube(hkArray<hkVector4> *verts, hkVector4 *scale, hkVector4 *offset);
void hkGeomConvexHullBuilder_buildConvexSubHull(const hkGeomConvexHullTolerances *tolerances,
        hkArray<hkVector4> *verts, int startVertex, int endVertex, hkGeomHull *hullOut);
void hkGeomConvexHullBuilder_removeUnusedVertices(hkGeomHull *hull, hkArray<hkVector4> *verts);
void hkGeomConvexHullBuilder_removeFlaggedVertices(hkArray<hkVector4> *verts);
void hkGeomConvexHullBuilder_postFilterVertices(hkGeomHull *hull, int startVertex, int endVertex,
        const hkGeomConvexHullTolerances *tolerances, hkBool *changedOut);
void hkGeomConvexHullBuilder_createBevelPlane(const hkVector4 *planeNormal, const hkVector4 *vertex0,
        const hkVector4 *vertex1, const hkVector4 *vertex2, hkArray<hkVector4> *planeEquationsOut);
void hkGeomConvexHullBuilder_PlaneAndPoints_sort(hkGeomConvexHullBuilder_PlaneAndPoints *self);
hkBool *hkGeomConvexHullBuilder_PlaneAndPoints_findPair(hkBool *result, hkGeomEdge *edgeBase,
        hkGeomEdge *p11, hkGeomEdge *p21, hkGeomEdge *p12, hkGeomEdge *p22,
        hkGeomConvexHullBuilder_PlaneAndPoints *p1, hkGeomConvexHullBuilder_PlaneAndPoints *p2);
hkBool *hkGeomConvexHullTester_isValidPlanarHull(hkBool *result, hkArray<hkVector4> *initialVerts,
        hkGeomHull *hull, hkArray<hkVector4> *usedVerts, hkArray<hkVector4> *planeEquations,
        hkArray<hkGeomConvexHullBuilder_PlaneAndPoints> *tangentPlanes, float coplanarTolerance);
hkBool *hkGeomConvexHullTester_isValidNonPlanarHull(hkBool *result, hkArray<hkVector4> *initialVerts,
        hkGeomHull *hull, hkArray<hkVector4> *usedVerts, hkArray<hkVector4> *planeEquations,
        hkArray<hkGeomConvexHullBuilder_PlaneAndPoints> *tangentPlanes, float coplanarTolerance);

/* ---- next-frontier externs (sibling builder/tester helpers, one more layer) ---- */
extern void hkGeomConvexHullBuilder_getAabb(hkArray<hkVector4> *verts, hkAabb *aabbOut);
extern void hkGeomConvexHullBuilder_mergeHulls(const hkGeomConvexHullTolerances *tolerances,
        hkGeomHull *hullA, hkGeomHull *hullB, hkGeomHull *hullOut);
extern void hkGeomHull_initializeWithVertex(hkGeomHull *hull, int v0);
extern void hkGeomHull_initializeWithEdge(hkGeomHull *hull, int v0, int v1);
extern void hkGeomHull_initializeWithTriangle(hkGeomHull *hull, int v0, int v1, int v2);
extern hkBool *hkGeomConvexHullTester_checkPlaneEquations(hkBool *result, hkArray<hkVector4> *initialVerts,
        hkArray<hkVector4> *usedVertices, hkArray<hkVector4> *planeEquations, float coplanarTolerance);
extern hkBool *hkGeomConvexHullTester_findSameEdges(hkBool *result,
        hkGeomConvexHullBuilder_PlaneAndPoints *planeA, hkGeomConvexHullBuilder_PlaneAndPoints *planeB,
        hkBool *foundA, hkBool *foundB, hkBool *foundC);
/* hkInplaceArray<hkGeomEdge,128>::operator= — deep-copy assignment. */
extern void hkInplaceArray_hkGeomEdge_128_assign(void *dst, const void *src);

/* ---- SDK-floor externs (stay boundary; do not descend) ---- */
extern void hkMatrix3_mul(hkMatrix3 *self, const hkMatrix3 *rhs);          /* hkMatrix3::mul (this = this * rhs) */
extern void hkArrayUtil__reserve(void *arrayData, int capacity, int elementSize);   /* arg0 void* per ?_reserve@hkArrayUtil@@YAXPAXHH@Z */
extern void hkArrayUtil__reserveMore(void *arrayData, int elementSize);             /* arg0 void* per ?_reserveMore@hkArrayUtil@@YAXPAXH@Z */
extern const hkVector4 hkVector4Zero;
