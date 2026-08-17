/* createDeformableShape @0x8383F840 */
#include "../headers/havok/hkdBreakableShape.h"
#include "../headers/havok/hkdDeformableBreakableShape.h"
#include "../headers/havok/hkdController.h"
#include "../headers/havok/hkdShapeInstanceInfo.h"
#include "../headers/havok/hkdBreakableShape_Connection.h"
#include "../headers/havok/hkdControllerUtil_ConnectionRef.h"
#include "../headers/havok/hkArray.h"
#include "../headers/havok/hkClass.h"
#include "../headers/havok/hkBool.h"
#include "../headers/havok/hkThreadMemory.h"
#include "../headers/havok/hkTransform.h"
#include "../headers/havok/hkError.h"
#include "../headers/havok/hkpShape.h"
#include "../headers/havok/hkpShapeType.h"
#include "../headers/havok/hkpConvexTransformShapeBase.h" /* hkpSingleShapeContainer */
#include "../headers/havok/hkReferencedObject.h"

#ifndef NULL
#define NULL nullptr
#endif

/* Forward-declared boundary pointer targets (used only as opaque pointers). */
typedef struct hkpListShape hkpListShape;
typedef struct hkpCollisionDispatcher hkpCollisionDispatcher;

/* Havok reflection identity of the deformable shape class + world identity. */
extern const hkClass hkdDeformableBreakableShapeClass;
extern const hkTransform hkTransformIdentity;

/* hkdCompoundBreakableShape::ConstructorFlags::CTR_FLAGS_SET_CHILD_PARENT == 2
   (DB exposes the enumerator name, not its numeric value). */
extern const int CTR_FLAGS_SET_CHILD_PARENT;

/* Boundaries -- Havok SDK, not reversed. */
extern const hkClass *hkdBreakableShape_getClassType(const hkdBreakableShape *self); /* virtual (vtable+4) */
extern hkBool *hkClass_isSuperClass(hkBool *out, const hkClass *self, const hkClass *other); /* hkClass::isSuperClass(hkClass const &) const, hkBool sret */
extern void hkdBreakableShape_getLeafShapes(const hkdBreakableShape *self, const hkTransform *parentTransform, hkArray<hkdShapeInstanceInfo> *leafShapesOut); /* hkdBreakableShape::getLeafShapes */
extern void hkdControllerUtil_findConnections(const hkdShapeInstanceInfo *shapesIn, int numShapesIn, hkArray<hkdControllerUtil_ConnectionRef> *connectionsOut, void *shapesMapOut); /* hkdControllerUtil::findConnections */
extern void hkdDeformableBreakableShape_construct(hkdDeformableBreakableShape *self, const hkdBreakableShape *oldParent, const hkdShapeInstanceInfo *childBreakableShapes, int numChildBreakableShapes, int flags); /* hkdDeformableBreakableShape::hkdDeformableBreakableShape */
extern void hkdBreakableShape_addConnection(hkdBreakableShape *self, const hkdBreakableShape_Connection *connection); /* hkdBreakableShape::addConnection */
extern void hkdAssetProcessingUtil_createShapeInstanceInfosFromPhysicsShape(const hkpListShape *listShape, hkArray<hkdShapeInstanceInfo> *shapeInfosOut); /* hkdAssetProcessingUtil::createShapeInstanceInfosFromPhysicsShape */
extern void hkReferencedObject_removeReferences(void *objects, int numObjects, int pointerStriding); /* hkReferencedObject::removeReferences */
extern void hkdAssetProcessingUtil_findConnectivityAll(hkpCollisionDispatcher *dispatcher, const hkdBreakableShape *shape, float distanceForConnection, float minArea); /* hkdAssetProcessingUtil::findConnectivityAll */
extern void hkdBreakableShape_setMassRecursively(hkdBreakableShape *self, float newMass, float inertiaTensorFactor); /* hkdBreakableShape::setMassRecursively */
extern void hkdBreakableShape_setStrengthRecursively(hkdBreakableShape *self, float strength, float relativeSubpieceStrength, int recalcChildren); /* hkdBreakableShape::setStrengthRecursively */
extern void hkdBreakableShape_setGraphicsShape(hkdBreakableShape *self, const hkdGraphicsShape *graphicsShape); /* hkdBreakableShape::setGraphicsShape */

/* Wraps an existing breakable shape as a deformable (soft/skinned destructible)
   breakable shape, for a controller of the deformable type. Two source paths:
   when the breakable shape already has child pieces we lift its leaf shapes and
   re-derive their connectivity; otherwise we synthesize children from the
   physics shape (which must resolve to an hkpListShape). Returns NULL when there
   is no controller, the controller is not deformable, or the shape is already a
   deformable breakable shape. Mirrors createBreakableShapeFromBlueprint but for
   the deformable path. */
hkdDeformableBreakableShape *createDeformableShape(
    const hkdBreakableShape *breakableShape,
    hkdController *controller,
    hkpCollisionDispatcher *dispatcher,
    float distanceForConnection)
{
    const hkClass *shapeClass;
    hkBool alreadyDeformable;
    hkReferencedObject *chunk;
    hkdBreakableShape *deformableShape;

    if (!controller)
        return NULL;
    if (controller->m_type.m_storage != CONTROLLER_TYPE_DEFORMATION)
        return NULL;

    /* Refuse to re-wrap a shape that is already a deformable breakable shape. */
    shapeClass = hkdBreakableShape_getClassType(breakableShape);
    hkClass_isSuperClass(&alreadyDeformable, &hkdDeformableBreakableShapeClass, shapeClass);
    if (alreadyDeformable.m_bool)
        return NULL;

    if (breakableShape->m_children.m_size)
    {
        /* Inline (stack-owned) array storage; capacityAndFlags top bit set marks
           "do not deallocate" until the array outgrows the inline buffer. */
        hkdShapeInstanceInfo leafInlineStorage[16];
        hkdControllerUtil_ConnectionRef connectionInlineStorage[1024];
        hkArray<hkdShapeInstanceInfo> leafShapes;
        hkArray<hkdControllerUtil_ConnectionRef> connectionRefs;
        const hkdControllerUtil_ConnectionRef *connectionRefData;
        int connectionIndex;

        leafShapes.m_data = leafInlineStorage;
        leafShapes.m_capacityAndFlags = 0x80000010; /* don't-deallocate | capacity 16 */
        leafShapes.m_size = 0;
        hkdBreakableShape_getLeafShapes(breakableShape, &hkTransformIdentity, &leafShapes);

        connectionRefs.m_size = 0;
        connectionRefs.m_data = connectionInlineStorage;
        connectionRefs.m_capacityAndFlags = 0x80000400; /* don't-deallocate | capacity 1024 */
        hkdControllerUtil_findConnections((const hkdShapeInstanceInfo *)leafShapes.m_data,
                                          leafShapes.m_size, &connectionRefs, NULL);

        chunk = (hkReferencedObject *)hkThreadMemory_allocateChunk(hkThreadMemory_getCurrent(),
                                                                   240, HK_MEMORY_CLASS_DESTRUCTION);
        chunk->m_memSizeAndFlags = 240;
        hkdDeformableBreakableShape_construct((hkdDeformableBreakableShape *)chunk, breakableShape,
                                              (const hkdShapeInstanceInfo *)leafShapes.m_data,
                                              leafShapes.m_size, CTR_FLAGS_SET_CHILD_PARENT);
        deformableShape = (hkdBreakableShape *)chunk;

        /* Copy each found connection (by value, 64B) into the deformable shape. */
        connectionRefData = (const hkdControllerUtil_ConnectionRef *)connectionRefs.m_data;
        for (connectionIndex = 0; connectionIndex < connectionRefs.m_size; ++connectionIndex)
        {
            hkdBreakableShape_Connection connection = *connectionRefData[connectionIndex].m_connection;
            hkdBreakableShape_addConnection(deformableShape, &connection);
        }

        if (connectionRefs.m_capacityAndFlags >= 0) /* heap-owned: release growth */
            hkThreadMemory_deallocateChunk(hkThreadMemory_getCurrent(), connectionRefs.m_data,
                                           8 * (connectionRefs.m_capacityAndFlags & 0x3FFFFFFF),
                                           HK_MEMORY_CLASS_ARRAY);
        if (leafShapes.m_capacityAndFlags >= 0)
            /* element = hkdShapeInstanceInfo (80B); compiler emitted 80*n as
               16*((n & 0x3FFFFFFF) + 4*n). */
            hkThreadMemory_deallocateChunk(hkThreadMemory_getCurrent(), leafShapes.m_data,
                                           80 * (leafShapes.m_capacityAndFlags & 0x3FFFFFFF),
                                           HK_MEMORY_CLASS_ARRAY);
    }
    else
    {
        const hkpShape *physicsShape = breakableShape->m_physicsShape.m_pntr;
        hkdShapeInstanceInfo shapeInfoInlineStorage[16];
        hkArray<hkdShapeInstanceInfo> shapeInfos;

        /* An HK_SHAPE_MOPP wrapper forwards to its inner shape: +0x30 is
           hkpMoppBvTreeShape.m_child (hkpSingleShapeContainer, DB types_members);
           the full hkpMoppBvTreeShape layout is not modeled (Havok boundary). */
        if (physicsShape->m_type.m_storage == HK_SHAPE_MOPP)
            physicsShape = ((const hkpSingleShapeContainer *)((const char *)physicsShape + 0x30))->m_childShape;

        if (physicsShape->m_type.m_storage != HK_SHAPE_LIST)
        {
            hkErrStream errStream;
            char messageBuffer[512];

            hkErrStream_construct(&errStream, messageBuffer, 512);
            hkOstream_insertString(&errStream.base,
                "Unsuccessful attempt to generate child breakable shapes from a physics shape. Physics shape must be a hkpListShape.");
            hkError_message(hkError_s_instance, MESSAGE_WARNING, 8413457, messageBuffer,
                "D:\\Projects\\code\\common\\lib_3dpart\\phys\\hk_include\\Destruction\\Destruction\\AssetProcessing\\hkdAssetProcessingUtil.cpp",
                1694);
            hkOstream_destruct(&errStream.base);
            return NULL;
        }

        shapeInfos.m_data = shapeInfoInlineStorage;
        shapeInfos.m_size = 0;
        shapeInfos.m_capacityAndFlags = 0x80000010; /* don't-deallocate | capacity 16 */
        hkdAssetProcessingUtil_createShapeInstanceInfosFromPhysicsShape((const hkpListShape *)physicsShape,
                                                                        &shapeInfos);

        chunk = (hkReferencedObject *)hkThreadMemory_allocateChunk(hkThreadMemory_getCurrent(),
                                                                   240, HK_MEMORY_CLASS_DESTRUCTION);
        chunk->m_memSizeAndFlags = 240;
        hkdDeformableBreakableShape_construct((hkdDeformableBreakableShape *)chunk, breakableShape,
                                              (const hkdShapeInstanceInfo *)shapeInfos.m_data,
                                              shapeInfos.m_size, CTR_FLAGS_SET_CHILD_PARENT);
        deformableShape = (hkdBreakableShape *)chunk;

        /* The constructor took a reference on each child shape; drop the local
           references held by the shape-info array (stride 80, m_shape at +64). */
        hkReferencedObject_removeReferences(&((hkdShapeInstanceInfo *)shapeInfos.m_data)->m_shape,
                                            shapeInfos.m_size, 80);
        hkdAssetProcessingUtil_findConnectivityAll(dispatcher, deformableShape, distanceForConnection,
                                                   9.9999997e-05f /* 0x38d1b717 */);
        hkdBreakableShape_setMassRecursively(deformableShape, breakableShape->m_massProps.m_mass, 1.0f);

        if (shapeInfos.m_capacityAndFlags >= 0)
            hkThreadMemory_deallocateChunk(hkThreadMemory_getCurrent(), shapeInfos.m_data,
                                           80 * (shapeInfos.m_capacityAndFlags & 0x3FFFFFFF),
                                           HK_MEMORY_CLASS_ARRAY);
    }

    hkdBreakableShape_setStrengthRecursively(deformableShape, breakableShape->m_strength, 0.5f, 0 /* RecalcChildren */);
    hkdBreakableShape_setGraphicsShape(deformableShape, breakableShape->m_graphicsShape.m_pntr);
    deformableShape->m_graphicsShapeName = breakableShape->m_graphicsShapeName;
    return (hkdDeformableBreakableShape *)deformableShape;
}
