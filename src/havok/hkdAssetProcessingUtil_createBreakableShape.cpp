/* ?createBreakableShape@hkdAssetProcessingUtil@@SAPAVhkdBreakableShape@@PBDPBVhkdGraphicsShape@@PBVhkpShape@@PAVhkdGraphicsSystem@@_N4@Z @0x8383A360 */
#include "../headers/havok/hkdBreakableShape.h"
#include "../headers/havok/hkdGraphicsShape.h"
#include "../headers/havok/hkdGraphicsSystem.h"
#include "../headers/havok/hkClass.h"
#include "../headers/havok/hkBool.h"
#include "../headers/havok/hkResult.h"
#include "../headers/havok/hkError.h"
#include "../headers/havok/hkThreadMemory.h"
#include "../headers/havok/hkpShape.h"
#include "../headers/havok/hkReferencedObject.h"
#include "../headers/havok/hkdMeshToGeometryConverter_boundary.h"

/* Boundary callees/globals (2-level descent limit). */
typedef struct hkMeshShape hkMeshShape;
typedef struct hkdGeometry hkdGeometry;
typedef struct hkpConvexVerticesShape hkpConvexVerticesShape; /* opaque; DEF takes hkpConvexVerticesShape* */
extern hkResult hkpConvexVerticesConnectivityUtil_ensureConnectivityAll(hkpConvexVerticesShape *shapeIn);
extern const hkClass *hkdGraphicsShape_getClassType(const hkdGraphicsShape *self);      /* virtual */
extern hkBool *hkClass_isSuperClass(hkBool *out, const hkClass *self, const hkClass *other); /* implemented in-corpus */
extern const hkClass hkdMeshGraphicsShapeClass;
extern hkMeshShape *hkdMeshGraphicsShape_getMesh(const hkdGraphicsShape *self);          /* *(void**)(self+sizeof(hkdGraphicsShape)) */
extern hkdGeometry *hkdMeshToGeometryConverter_convertMeshToGeometry(const hkdMeshToGeometryConverter_Input *input, const hkMeshShape *shape, const char *name);
extern hkResult hkdGeometry_checkConsistency(hkdGeometry *geometry);
extern void hkdExceptionReporter_startCatching(void);
extern void hkdExceptionReporter_stopCatching(int reportMode);
extern const int REPORT_PERFRAME_SUMMARY;
extern void hkdBreakableShape_construct(hkdBreakableShape *self, const hkpShape *physicsShape, const void *parentName, const hkdGeometry *geometry);
extern void hkdBreakableShape_setGraphicsShape(hkdBreakableShape *self, const hkdGraphicsShape *graphicsShape); /* implemented in-corpus */
extern void hkdGraphicsSystem_registerShape(hkdGraphicsSystem *self, const hkdGraphicsShape *shape, const char *name); /* virtual */
extern const int HK_MEMORY_CLASS_DESTRUCTION;

#define ASSET_UTIL_CPP "D:\\Projects\\code\\common\\lib_3dpart\\phys\\hk_include\\Destruction\\Destruction\\AssetProcessing\\hkdAssetProcessingUtil.cpp"

/* hkdAssetProcessingUtil::createBreakableShape — build a runtime breakable shape
   from a physics shape (+ optional graphics mesh converted to fracture geometry).
   Returns null if the convex hull is inconsistent. */
hkdBreakableShape *hkdAssetProcessingUtil_createBreakableShape(const char *name,
        const hkdGraphicsShape *graphicsShape, const hkpShape *physicsShape,
        hkdGraphicsSystem *graphicsSystem, bool mergeCoplanarTriangles, bool createGeometry)
{
    char buffer[512];
    hkdGeometry *geometry = 0;
    hkdBreakableShape *breakable;

    if (hkpConvexVerticesConnectivityUtil_ensureConnectivityAll((hkpConvexVerticesShape *)physicsShape) == HK_FAILURE)
    {
        hkErrStream es;
        hkErrStream_construct(&es, buffer, 512);
        hkOstream_insertString(hkOstream_insertString(hkOstream_insertString(&es.base, "Object '"), name),
                               "' has a inconsistent convex hull, try to simplify the physics representation");
        hkError_message(hkError_s_instance, MESSAGE_WARNING, -1413812171, buffer, ASSET_UTIL_CPP, 630);
        hkOstream_destruct(&es.base);
        return 0;
    }

    if (createGeometry)
    {
        hkBool isMesh;
        if (graphicsShape)
            hkClass_isSuperClass(&isMesh, &hkdMeshGraphicsShapeClass, hkdGraphicsShape_getClassType(graphicsShape));
        else
            isMesh.m_bool = 0;

        if (graphicsShape && isMesh.m_bool)
        {
            /* the merge-coplanar flag rides in the converter Input struct (arg0);
               the mesh is arg1 per the real DEF (was modelled as an hkBool* + mesh). */
            hkdMeshToGeometryConverter_Input converterInput = {};
            converterInput.m_mergeCoplanarTriangles = (unsigned char)mergeCoplanarTriangles;
            hkResult consistency;
            hkdExceptionReporter_startCatching();
            geometry = hkdMeshToGeometryConverter_convertMeshToGeometry(
                &converterInput, hkdMeshGraphicsShape_getMesh(graphicsShape), name);
            consistency = hkdGeometry_checkConsistency(geometry);
            hkdExceptionReporter_stopCatching(REPORT_PERFRAME_SUMMARY);
            if (consistency != HK_SUCCESS)
            {
                hkErrStream es;
                hkErrStream_construct(&es, buffer, 512);
                hkOstream_insertString(hkOstream_insertString(hkOstream_insertString(&es.base, "Object '"), name),
                                       "' is not a valid closed mesh - no geometry produced\n");
                hkError_message(hkError_s_instance, MESSAGE_WARNING, -1413812172, buffer, ASSET_UTIL_CPP, 655);
                hkOstream_destruct(&es.base);
                hkReferencedObject_removeReference((hkReferencedObject *)geometry);
                geometry = 0;
            }
        }
        else
        {
            hkErrStream es;
            hkErrStream_construct(&es, buffer, 512);
            hkOstream_insertString(&es.base,
                "Your supplied graphics shape is not of type 'hkdMeshGraphicsShape', cannot fracture this object");
            hkError_message(hkError_s_instance, MESSAGE_WARNING, -1413857325, buffer, ASSET_UTIL_CPP, 641);
            hkOstream_destruct(&es.base);
        }
    }

    breakable = (hkdBreakableShape *)hkThreadMemory_allocateChunk(
        hkThreadMemory_getCurrent(), 208, HK_MEMORY_CLASS_DESTRUCTION);
    ((hkReferencedObject *)breakable)->m_memSizeAndFlags = 208;
    hkdBreakableShape_construct(breakable, physicsShape, 0, geometry);

    if (geometry)
    {
        hkReferencedObject_removeReference((hkReferencedObject *)geometry);
        if (graphicsSystem)
        {
            hkdBreakableShape_setGraphicsShape(breakable, graphicsShape);
            breakable->m_graphicsShapeName = name;
            hkdGraphicsSystem_registerShape(graphicsSystem, graphicsShape, name);
        }
    }
    return breakable;
}
