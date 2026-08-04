#pragma once
#include "hkVector4.h"
#include "hkTransform.h"
#include "hkArray.h"
#include "hkStridedVertices.h"
#include "hkThreadMemory.h"
#include "hkpConvexVerticesShape.h"
#include "hkGeometry.h"

/* Shared boundary declarations for the hkpShapeCutterUtil convex-hull builders
   (createSphere/Cylinder/Aabb/Capsule ConvexVerticesShape). These callees are
   left as externs — they are outside this re-source slice. */

/* memory-class tag used by these builders (enumerator name only in the DB). */
extern const int HK_MEMORY_CLASS_CDINFO;

/* hkArray growth helpers (hkArrayUtil::). First arg is &array.m_data. */
extern void hkArrayUtil__reserveMore(void *arrayData, int elementSize);            /* hkArrayUtil::_reserveMore */
extern void hkArrayUtil__reserve(void *arrayData, int capacity, int elementSize);  /* hkArrayUtil::_reserve */

/* Face emitters: append a face's vertex-index list to `connectivity` and the
   face plane equation to `planeEquationsOut`, sourcing positions from `vertices`.
   idx points to 3 (triangle) or 4 (quad) vertex indices. */
extern void hkpShapeCutterUtil__addTriangle(const int *idx, const hkpConvexVerticesConnectivity *connectivity,
                                            const hkArray<hkVector4> *vertices, hkArray<hkVector4> *planeEquationsOut); /* hkpShapeCutterUtil::_addTriangle */
extern void hkpShapeCutterUtil__addQuad(const int *idx, const hkpConvexVerticesConnectivity *connectivity,
                                        const hkArray<hkVector4> *vertices, hkArray<hkVector4> *planeEquationsOut);     /* hkpShapeCutterUtil::_addQuad */

/* hkpConvexVerticesShape construction. */
extern void *hkpConvexVerticesConnectivity_vftable;   /* hkpConvexVerticesConnectivity::`vftable' */
extern const float hkConvexShapeDefaultRadius;
extern void hkpConvexVerticesShape_construct(hkpConvexVerticesShape *self, const hkStridedVertices *vertices,
                                             const hkArray<hkVector4> *planeEquations, float radius); /* hkpConvexVerticesShape::hkpConvexVerticesShape */

/* vector / geometry helpers */
extern void hkVector4_setTransformedPos(hkVector4 *out, const hkTransform *t, const hkVector4 *pos); /* hkVector4::setTransformedPos */
extern void hkVector4_setRotatedDir(hkVector4 *out, const hkRotation *rot, const hkVector4 *dir);    /* hkVector4::setRotatedDir */
extern void hkArrayUnsignedShort_spliceInto(hkArray<unsigned short> *dst, int index, int numToRemove,
                                            const unsigned short *src, int numToInsert);             /* hkArray<unsigned short>::spliceInto */
extern void hkGeometryUtils_createCapsuleGeometry(const hkVector4 *top, const hkVector4 *bottom, float radius,
                                                  int numSides, int numHeightSegments, const hkTransform *transform,
                                                  hkGeometry **geometryOut);                          /* hkGeometryUtils::createCapsuleGeometry */
extern void hkGeometry_destruct(hkGeometry *self);                                                    /* hkGeometry::~hkGeometry */

/* Helper: allocate + initialise an empty hkpConvexVerticesConnectivity. */
static hkpConvexVerticesConnectivity *hkpShapeCutterUtil_newConnectivity(void)
{
    hkpConvexVerticesConnectivity *c =
        (hkpConvexVerticesConnectivity *)hkThreadMemory_allocateChunk(hkThreadMemory_getCurrent(), 32, HK_MEMORY_CLASS_CDINFO);
    c->base.__vftable = (hkBaseObject_vtbl *)&hkpConvexVerticesConnectivity_vftable;
    c->base.m_memSizeAndFlags = 32;
    c->base.m_referenceCount = 1;
    c->m_vertexIndices.m_data = nullptr;
    c->m_vertexIndices.m_size = 0;
    c->m_vertexIndices.m_capacityAndFlags = 0x80000000;
    c->m_numVerticesPerFace.m_data = nullptr;
    c->m_numVerticesPerFace.m_size = 0;
    c->m_numVerticesPerFace.m_capacityAndFlags = 0x80000000;
    return c;
}

/* Helper: append a single vertex to an hkArray<hkVector4>, growing if needed. */
static void hkpShapeCutterUtil_pushVertex(hkArray<hkVector4> *arr, const hkVector4 *v)
{
    if (arr->m_size == (arr->m_capacityAndFlags & 0x3FFFFFFF))
        hkArrayUtil__reserveMore(&arr->m_data, 16);
    arr->m_data[arr->m_size++] = *v;
}

/* Helper: build the final shape from accumulated vertices + plane equations,
   attach connectivity, drop the connectivity's build reference, and free the
   two working arrays. Returns the new shape. */
static hkpConvexVerticesShape *hkpShapeCutterUtil_finishShape(const hkArray<hkVector4> *vertices,
                                                              hkArray<hkVector4> *planeEquations,
                                                              const hkpConvexVerticesConnectivity *connectivity)
{
    hkThreadMemory *mem = hkThreadMemory_getCurrent();
    hkStridedVertices strided;
    hkpConvexVerticesShape *shape;

    strided.m_striding = 16;
    strided.m_vertices = vertices->m_data->m_quad.___u0.vector4_f32;
    strided.m_numVertices = vertices->m_size;

    shape = (hkpConvexVerticesShape *)hkThreadMemory_allocateChunk(mem, 96, HK_MEMORY_CLASS_CDINFO);
    shape->base.m_memSizeAndFlags = 96; /* inherited hkpShape::base (hkReferencedObject) */
    hkpConvexVerticesShape_construct(shape, &strided, planeEquations, hkConvexShapeDefaultRadius);
    hkpConvexVerticesShape_setConnectivity(shape, connectivity);
    hkReferencedObject_removeReference((hkReferencedObject *)connectivity);

    if ((int)planeEquations->m_capacityAndFlags >= 0)
        hkThreadMemory_deallocateChunk(mem, planeEquations->m_data,
                                       16 * planeEquations->m_capacityAndFlags, HK_MEMORY_CLASS_ARRAY);
    return shape;
}
