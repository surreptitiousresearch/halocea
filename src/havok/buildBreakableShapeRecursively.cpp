/* buildBreakableShapeRecursively @0x8383DA10 */
#include "../headers/havok/hkResourceContainer.h"
#include "../headers/havok/hkpShapeInfo.h"
#include "../headers/havok/hkpShape.h"
#include "../headers/havok/hkStringMap.h"
#include "../headers/havok/hkpTransformCollapseUtil_boundary.h"
#include "../headers/havok/hkdAssetProcessingUtil_Output.h"
#include "../headers/havok/hkdShapeInstanceInfo.h"
#include "../headers/havok/hkdBreakableShape.h"
#include "../headers/havok/hkdCompoundBreakableShape.h"
#include "../headers/havok/hkdShape.h"
#include "../headers/havok/hkdBody.h"
#include "../headers/havok/hkpListShape.h"
#include "../headers/havok/hkpMoppBvTreeShape.h"
#include "../headers/havok/hkpShapeCollection.h"
#include "../headers/havok/hkdFracture.h"
#include "../headers/havok/FractureInfo.h"
#include "../headers/havok/hkdAssetProcessingUtil_GraphicsNode.h"
#include "../headers/havok/hkdGraphicsShape.h"
#include "../headers/havok/hkdGraphicsSystem.h"
#include "../headers/havok/hkdAction.h"
#include "../headers/havok/hkTransform.h"
#include "../headers/havok/hkArray.h"
#include "../headers/havok/hkThreadMemory.h"
#include "../headers/havok/hkError.h"
#include "../headers/havok/hkReferencedObject.h"
#include "../headers/havok/hkpCollisionDispatcher.h"

/*
 * CAVEAT (semantic uncertainty documented per never-stop drain policy):
 *  - Reconstructed from a VMX128-dense, reg-alloc-heavy decompile. Every hkTransform
 *    move that the decompiler emitted as raw lvx128/stvx128 __asm blocks has been
 *    restored to a scalar `hkTransform` struct assignment, verified against the
 *    disasm (source/dest column offsets 0/16/32/48). The prologue loads
 *    shapeInfo->m_transform into non-volatile VMX regs (v123..v126, saved by
 *    __savevmx_123) and the epilogue stores them as the appended instance's
 *    m_transform -- rendered here as `newInstance->m_transform = shapeInfo->m_transform`.
 *  - The decompiler packed two unrelated uses into one 8-byte stack slot (var_478):
 *    first the recursion's child FractureInfo, then a scratch `hkpShape*` output for
 *    the transform-collapse. These are split into `childFractureInfo` and
 *    `transformedChildShape` here (disasm-confirmed: std of the full 8-byte v110 for
 *    the FractureInfo; a 4-byte hkpShape* write at &slot for collapseTransforms).
 *  - The per-mesh child physics-shape enumeration walks hkpListShape internals
 *    (embedded hkpShapeContainer at listShape+16, vtable slot +4 = getNumChildShapes,
 *    child-key array pointer at listShape+24, stride 16). No DB struct covers these,
 *    so raw-offset dispatch is kept with comments. Physics-shape type tags 9/10 are
 *    hkpShapeType enumerators (9 = a child-list container; 10 = a wrapper holding a
 *    list child at +52), kept numeric.
 *  - hkThreadMemory_getCurrent() models the TLS deref *(hkThreadMemory**)(*(int*)r13
 *    + 704) per the established corpus convention.
 */

#define HKD_ASSET_PROCESSING_UTIL_CPP \
    "D:\\Projects\\code\\common\\lib_3dpart\\phys\\hk_include\\Destruction\\Destruction\\AssetProcessing\\hkdAssetProcessingUtil.cpp"

/* hkClass reflection descriptors selecting resources by type. DB exposes the
   symbol names; hkpRigidBodyClass and hkpShapeInfoClass are declared in
   hkResourceContainer.h. Boundary. */
extern const hkClass hkdBodyClass;
extern const hkClass hkdShapeClass;
extern const hkClass hkdFractureClass;
extern const hkClass hkdActionClass;

/* hkdCompoundBreakableShape::ConstructorFlags enumerator (DB name, not value). */
extern const int CTR_FLAGS_SET_CHILD_PARENT;

/* Boundaries -- Havok SDK / other reversed siblings, not re-sourced here. */
extern const char *hkClass_getName(const hkClass *self); /* hkClass::getName */
extern void *hkStringMap_getWithDefault(const hkStringMapBase *map, unsigned int key, void *defaultValue);
extern void hkArrayUtil__reserveMore(void *array, int elementSize);           /* hkArrayUtil::_reserveMore */
extern void hkTransform_setInverse(hkTransform *out, const hkTransform *in);  /* hkTransform::setInverse */
extern void _flattenRecursiveCompounds(hkArray_hkdShapeInstanceInfo *shapes);
extern void hkdCompoundBreakableShape_construct(hkdCompoundBreakableShape *self,
                                                const hkdBreakableShape *oldParent,
                                                const hkdShapeInstanceInfo *children,
                                                int numChildren, int constructorFlags);
extern void hkdBreakableShape_setSimpleValuesFromBlueprint(hkdBreakableShape *self, const hkdShape *shapeBp);
extern void hkdBreakableShape_addChild(hkdBreakableShape *self, const hkdBreakableShape *child,
                                       const hkTransform *transform);
extern void hkdBreakableShape_setSimpleValuesFromChildren(hkdBreakableShape *self,
                                                          const hkdShapeInstanceInfo *children, int numChildren);
extern void hkdBreakableShape_setGraphicsShape(hkdBreakableShape *self, const hkdGraphicsShape *graphicsShape);
extern void hkdBreakableShape_addAction(hkdBreakableShape *self, hkdAction *action);

extern hkdBreakableShape *createBreakableShapeFromBlueprint(hkdShape *shapeBp, const hkpShape *physicsShape,
                                                           FractureInfo *fractureInfo,
                                                           hkdAssetProcessingUtil_GraphicsNode *graphicsNode,
                                                           hkdGraphicsSystem *graphicsSystem,
                                                           unsigned char isDeformable);
extern void setBreakableShapeFracture(const FractureInfo *fractureInfo, hkdBreakableShape *breakableShape,
                                      const hkdAssetProcessingUtil_GraphicsNode *graphicsNode,
                                      hkArray<> *fractureTasksOut, unsigned char isDeformable);
extern void hkArray_spliceInto(hkArray<hkdShapeInstanceInfo> *self, int index, int numToRemove, void *insertData,
                               int numToInsert); /* hkArray::spliceInto */

/* Recursively build the hkdBreakableShape tree for one resource container (and
   its nested child containers) out of the physics-shape / graphics-node blueprint
   data, appending the resulting shape instances to shapeInstancesOut. */
void buildBreakableShapeRecursively(hkResourceContainer *container,
                                    hkpCollisionDispatcher *dispatcher,
                                    hkdGraphicsSystem *graphicsSystem,
                                    const hkStringMap *graphicsNodes,
                                    FractureInfo *fractureInfoIn,
                                    int level,
                                    hkdAssetProcessingUtil_Output *output,
                                    hkArray_hkdShapeInstanceInfo *shapeInstancesOut)
{
    char nameBuffer[32];
    const char *containerName = hkResourceContainer_getName(container, nameBuffer);

    /* Warn about (and ignore) a nested rigid body -- unsupported by Destruction. */
    if (level > 0)
    {
        hkResourceHandle *rigidBodyHandle =
            hkResourceContainer_findResourceByName(container, 0, &hkpRigidBodyClass, 0);
        if (rigidBodyHandle && hkResourceHandle_getObject(rigidBodyHandle))
        {
            hkErrStream stream;
            char errBuffer[592];

            hkErrStream_construct(&stream, errBuffer, 512);
            hkOstream_insertString((hkOstream *)&stream, "Found a nested rigid body '");
            hkOstream_insertString((hkOstream *)&stream, containerName);
            hkOstream_insertString((hkOstream *)&stream,
                "'. This is not supported by Havok Destruction. The nested rigid body modifier will be ignored.");
            hkError_message(hkError_s_instance, MESSAGE_WARNING, 0xABBA34DA, errBuffer,
                            HKD_ASSET_PROCESSING_UTIL_CPP, 1045);
            hkOstream_destruct((hkOstream *)&stream);
        }
    }

    /* An hkdBody at this container makes the shape deformable when its blueprint's
       type == 2 (deformable). Raw offsets: hkdBody+12 = breakable-shape blueprint,
       blueprint+8 = type tag -- no DB struct covers hkdBody here. */
    /* DEVIATION: local declared as unsigned char (not the hkBool wrapper): the DB
       treats this purely as a 0/1 flag and passes it to unsigned-char params. */
    unsigned char isDeformable = 0;
    {
        hkResourceHandle *bodyHandle = hkResourceContainer_findResourceByName(container, 0, &hkdBodyClass, 0);
        hkdBody *bodyObject = bodyHandle ? (hkdBody *)hkResourceHandle_getObject(bodyHandle) : 0;
        if (bodyObject)
        {
            /* typed (DB types_members hkdBody::m_controller@12 / hkdController::m_type@8);
               was raw +12/+8 reads of an untyped "blueprint". */
            hkdController *bodyController = bodyObject->m_controller.m_pntr;
            if (bodyController)
                isDeformable = bodyController->m_type.m_storage == CONTROLLER_TYPE_DEFORMATION;
        }
    }

    hkdAssetProcessingUtil_GraphicsNode *containerGraphicsNode =
        (hkdAssetProcessingUtil_GraphicsNode *)hkStringMap_getWithDefault(&graphicsNodes->m_map,
                                                                          (unsigned int)containerName, 0);

    /* Resolve the shape blueprint and, when present, adopt the first unlinked
       fracture found in this container -- chaining any further fractures onto the
       previous one's m_childFracture. */
    hkdShape *shapeBlueprint = 0;
    {
        hkResourceHandle *shapeHandle = hkResourceContainer_findResourceByName(container, 0, &hkdShapeClass, 0);
        if (shapeHandle)
        {
            shapeBlueprint = (hkdShape *)hkResourceHandle_getObject(shapeHandle);
            if (shapeBlueprint && !shapeBlueprint->m_fracture.m_pntr)
            {
                hkdFracture *previousFracture = 0;
                const hkResourceHandle *fractureHandle;
                for (fractureHandle = hkResourceContainer_findResourceByName(container, 0, &hkdFractureClass, 0);
                     fractureHandle;
                     fractureHandle = hkResourceContainer_findResourceByName(container, 0, &hkdFractureClass,
                                                                            (void *)fractureHandle))
                {
                    hkdFracture *fractureObject = (hkdFracture *)hkResourceHandle_getObject(
                        (hkResourceHandle *)fractureHandle);
                    if (fractureObject == shapeBlueprint->m_dynamicFracture.m_pntr)
                        continue;

                    {
                        char fractureNameBuffer[32];
                        const char *fractureName = hkResourceHandle_getName(fractureHandle, fractureNameBuffer);

                        if (previousFracture)
                        {
                            /* hkRefPtr assignment: add-ref new, release previous, store. */
                            if (fractureObject)
                                hkReferencedObject_addReference(&fractureObject->base);
                            if (previousFracture->m_childFracture.m_pntr)
                                hkReferencedObject_removeReference(&previousFracture->m_childFracture.m_pntr->base);
                            previousFracture->m_childFracture.m_pntr = fractureObject;
                        }
                        else
                        {
                            hkErrStream stream;
                            char errBuffer[592];

                            hkErrStream_construct(&stream, errBuffer, 512);
                            hkOstream_insertString((hkOstream *)&stream, "Breakable shape '");
                            hkOstream_insertString((hkOstream *)&stream, containerName);
                            hkOstream_insertString((hkOstream *)&stream,
                                "' has no fracture linked, but found fracture '");
                            hkOstream_insertString((hkOstream *)&stream, fractureName);
                            hkOstream_insertString((hkOstream *)&stream,
                                "', which will be used. Link the fracture to remove this warning.");
                            hkError_message(hkError_s_instance, MESSAGE_WARNING, 0xABBA6578, errBuffer,
                                            HKD_ASSET_PROCESSING_UTIL_CPP, 1081);
                            hkOstream_destruct((hkOstream *)&stream);

                            if (fractureObject)
                                hkReferencedObject_addReference(&fractureObject->base);
                            if (shapeBlueprint->m_fracture.m_pntr)
                                hkReferencedObject_removeReference(&shapeBlueprint->m_fracture.m_pntr->base);
                            shapeBlueprint->m_fracture.m_pntr = fractureObject;
                        }
                        previousFracture = fractureObject;
                    }
                }
            }
        }
    }

    /* Compose the effective fracture info from the caller's plus any blueprint
       overrides. */
    FractureInfo fractureInfo = *fractureInfoIn;
    if (shapeBlueprint)
    {
        if (shapeBlueprint->m_fracture.m_pntr)
            fractureInfo.m_fracture = shapeBlueprint->m_fracture.m_pntr;
        if (shapeBlueprint->m_dynamicFracture.m_pntr)
            fractureInfo.m_dynamicFracture = shapeBlueprint->m_dynamicFracture.m_pntr;
    }

    /* Local accumulator of child shape instances. */
    hkArray_hkdShapeInstanceInfo localShapes;
    localShapes.m_data = 0;
    localShapes.m_size = 0;
    localShapes.m_capacityAndFlags = 0x80000000; /* "don't free" flag while empty */

    /* Fracture info handed to the recursion: a copy of the composed info, but
       cleared when this container itself owns a graphics node (its own mesh takes
       over, so children inherit no fracture). Shares var_478 with
       transformedChildShape below (disasm-confirmed slot reuse). */
    FractureInfo childFractureInfo = fractureInfo;
    if (containerGraphicsNode)
    {
        childFractureInfo.m_fracture = 0;
        childFractureInfo.m_dynamicFracture = 0;
    }

    /* Recurse into every nested child container. */
    hkResourceContainer *childContainer;
    for (childContainer = hkResourceContainer_findContainerByName(container, 0, 0);
         childContainer;
         childContainer = hkResourceContainer_findContainerByName(container, 0, childContainer))
    {
        buildBreakableShapeRecursively(childContainer, dispatcher, graphicsSystem, graphicsNodes,
                                       &childFractureInfo, level + 1, output, &localShapes);
    }

    /* Resolve the shape-info node describing the physics shape and its meshes. */
    const char *shapeInfoClassName = hkClass_getName(&hkpShapeInfoClass);
    hkpShapeInfo *shapeInfo = 0;
    {
        hkResourceHandle *shapeInfoHandle =
            hkResourceContainer_findResourceByName(container, shapeInfoClassName, &hkpShapeInfoClass, 0);
        if (shapeInfoHandle)
            shapeInfo = (hkpShapeInfo *)hkResourceHandle_getObject(shapeInfoHandle);
    }

    hkdBreakableShape *resultShape = 0;
    if (!shapeInfo)
    {
        if (shapeBlueprint || isDeformable)
        {
            hkErrStream stream;
            char errBuffer[592];

            hkErrStream_construct(&stream, errBuffer, 512);
            hkOstream_insertString((hkOstream *)&stream, "Missing hkpShapeInfo node for: '");
            hkOstream_insertString((hkOstream *)&stream, containerName);
            hkOstream_insertString((hkOstream *)&stream, "'");
            hkError_message(hkError_s_instance, MESSAGE_WARNING, 0xABBADFE4, errBuffer,
                            HKD_ASSET_PROCESSING_UTIL_CPP, 1129);
            hkOstream_destruct((hkOstream *)&stream);
        }
        else
        {
            /* No shape info and nothing deformable: hand accumulated children up. */
            hkArray_spliceInto((hkArray<hkdShapeInstanceInfo> *)shapeInstancesOut, shapeInstancesOut->m_size, 0,
                               localShapes.m_data, localShapes.m_size);
            localShapes.m_size = 0;
        }
        goto cleanup;
    }

    /* DEVIATION: declared without initializers so the earlier `goto cleanup`
       (C++ forbids jumping over an initialized declaration; C2362). */
    const hkpShape *physicsShape;
    unsigned int physicsShapeType;
    const hkpShape *listShape;
    physicsShape = shapeInfo->m_shape.m_pntr;
    physicsShapeType = physicsShape->m_type.m_storage;

    /* Select the child-list physics shape (if any): a list shape is itself the
       list; a MOPP shape wraps its list child in m_child (hkpSingleShapeContainer). */
    listShape = 0;
    if (physicsShapeType == HK_SHAPE_LIST)
    {
        listShape = shapeInfo->m_shape.m_pntr;
    }
    else if (physicsShapeType == HK_SHAPE_MOPP &&
             ((const hkpMoppBvTreeShape *)physicsShape)->m_child.m_childShape->m_type.m_storage == HK_SHAPE_LIST)
    {
        listShape = ((const hkpMoppBvTreeShape *)physicsShape)->m_child.m_childShape;
    }

    hkdShapeInstanceInfo_RuntimeInfo defaultRuntimeInfo;

    if (shapeInfo->m_isHierarchicalCompound.m_bool || !containerGraphicsNode)
    {
        /* COMPOUND path: build one breakable child per mesh, then wrap them in an
           hkdCompoundBreakableShape. */
        if (!shapeInfo->m_hkdShapesCollected.m_bool && listShape)
        {
            /* Embedded hkpShapeContainer subobject of hkpShapeCollection (vtable ptr @16);
               vtable slot +4 = getNumChildShapes(). Opaque hkpListShape internals. */
            char *shapeContainer = (char *)&((const hkpShapeCollection *)listShape)->m_container_vftable;
            typedef int (*GetNumChildShapesFn)(void *self);
            /* Opaque hkpShapeContainer vtable dispatch: vptr@+0, getNumChildShapes at vtable slot +4 */
            unsigned int *shapeContainerVtable = *(unsigned int **)shapeContainer;
            GetNumChildShapesFn getNumChildShapes =
                (GetNumChildShapesFn)(void *)shapeContainerVtable[1];

            if (getNumChildShapes(shapeContainer) == shapeInfo->m_childShapeNames.m_size)
            {
                int childIndex = 0;
                int childCount = getNumChildShapes(shapeContainer);
                while (childIndex < childCount)
                {
                    hkTransform childTransform;
                    hkTransform inverseChildTransform;
                    const hkpShape *transformedChildShape;
                    unsigned int graphicsNodeKey;

                    /* typed (DB hkpListShape::m_childInfo@24, ChildInfo stride 16, m_shape@0);
                       was a raw +24 pointer walk. */
                    const hkpListShape_ChildInfo *childInfoArray =
                        ((const hkpListShape *)listShape)->m_childInfo.m_data;
                    const hkpShape *childPhysicsShape = childInfoArray[childIndex].m_shape;

                    childTransform = ((hkTransform *)shapeInfo->m_childTransforms.m_data)[childIndex];
                    hkTransform_setInverse(&inverseChildTransform, &childTransform);

                    graphicsNodeKey =
                        (unsigned int)((const char **)shapeInfo->m_childShapeNames.m_data)[childIndex];

                    if (childIndex || !shapeInfo->m_isHierarchicalCompound.m_bool)
                    {
                        const hkpShape *collapsedShape =
                            hkpTransformCollapseUtil_transformTransformShape(childPhysicsShape, &inverseChildTransform);
                        if (childPhysicsShape == collapsedShape)
                        {
                            transformedChildShape = collapsedShape;
                        }
                        else
                        {
                            hkpTransformCollapseUtil_Options collapseOptions;
                            hkpTransformCollapseUtil_Results collapseResults;

                            hkpTransformCollapseUtil_Options_construct(&collapseOptions);
                            collapseOptions.m_propageTransformInList.m_bool = 1;
                            hkpTransformCollapseUtil_Results_construct(&collapseResults);
                            hkpTransformCollapseUtil_collapseTransforms(collapsedShape, &collapseOptions,
                                                                        &collapseResults, &transformedChildShape);
                            hkReferencedObject_removeReference(&((hkpShape *)collapsedShape)->base);
                        }
                    }
                    else
                    {
                        /* First child of a hierarchical compound: identity transform,
                           shape kept as-is, graphics node keyed by the container name.
                           DEVIATION: decompiler built the identity as VMX vxor+stvx;
                           restored to scalar diagonal writes. */
                        transformedChildShape = childPhysicsShape;
                        graphicsNodeKey = (unsigned int)containerName;

                        childTransform.m_rotation.m_col0.m_quad.___u0.v[0] = 0.0f;
                        childTransform.m_rotation.m_col0.m_quad.___u0.v[1] = 0.0f;
                        childTransform.m_rotation.m_col0.m_quad.___u0.v[2] = 0.0f;
                        childTransform.m_rotation.m_col0.m_quad.___u0.v[3] = 0.0f;
                        childTransform.m_rotation.m_col1 = childTransform.m_rotation.m_col0;
                        childTransform.m_rotation.m_col2 = childTransform.m_rotation.m_col0;
                        childTransform.m_translation = childTransform.m_rotation.m_col0;
                        childTransform.m_rotation.m_col0.m_quad.___u0.v[0] = 1.0f;
                        childTransform.m_rotation.m_col1.m_quad.___u0.v[1] = 1.0f;
                        childTransform.m_rotation.m_col2.m_quad.___u0.v[2] = 1.0f;
                    }

                    hkdAssetProcessingUtil_GraphicsNode *childGraphicsNode =
                        (hkdAssetProcessingUtil_GraphicsNode *)hkStringMap_getWithDefault(&graphicsNodes->m_map,
                                                                                          graphicsNodeKey, 0);
                    if (childGraphicsNode)
                    {
                        const hkdGraphicsShape *childGraphicsShape = childGraphicsNode->m_graphics.m_pntr;
                        hkdBreakableShape *childBreakableShape =
                            createBreakableShapeFromBlueprint(shapeBlueprint, transformedChildShape, &fractureInfo,
                                                              childGraphicsNode, graphicsSystem, isDeformable);
                        setBreakableShapeFracture(&fractureInfo, childBreakableShape, childGraphicsNode,
                                                  output->m_fractureTasks, isDeformable);
                        hkdBreakableShape_setGraphicsShape(childBreakableShape, childGraphicsShape);
                        childBreakableShape->m_graphicsShapeName = childGraphicsNode->m_name;

                        if (localShapes.m_size == (localShapes.m_capacityAndFlags & 0x3FFFFFFF))
                            hkArrayUtil__reserveMore(&localShapes, sizeof(hkdShapeInstanceInfo));

                        defaultRuntimeInfo.m_distanceToDestructionPoint = 3.40282347e38f;
                        defaultRuntimeInfo.m_oldChildIdx = 0xFFFF;
                        defaultRuntimeInfo.m_forceFixed = 0;

                        {
                            int newIndex = localShapes.m_size;
                            hkdShapeInstanceInfo *newInstance = &localShapes.m_data[newIndex];
                            localShapes.m_size = newIndex + 1;
                            newInstance->m_transform = childTransform;
                            newInstance->m_shape = childBreakableShape;
                            newInstance->m_damage = 0;
                            newInstance->m_runtimeInfo = defaultRuntimeInfo;
                        }
                    }
                    else
                    {
                        hkErrStream stream;
                        char errBuffer[592];

                        hkErrStream_construct(&stream, errBuffer, 512);
                        hkOstream_insertString((hkOstream *)&stream,
                            "The number of meshes does not match the number of physics shapes in '");
                        hkOstream_insertString((hkOstream *)&stream, containerName);
                        hkOstream_insertString((hkOstream *)&stream, "'");
                        hkError_message(hkError_s_instance, MESSAGE_WARNING, 0xABBAAFED, errBuffer,
                                        HKD_ASSET_PROCESSING_UTIL_CPP, 1236);
                        hkOstream_destruct((hkOstream *)&stream);
                    }

                    ++childIndex;
                    childCount = getNumChildShapes(shapeContainer);
                }
            }
        }

        if (!localShapes.m_size)
            goto search_shape_actions;

        _flattenRecursiveCompounds(&localShapes);

        hkdCompoundBreakableShape *compoundShape =
            (hkdCompoundBreakableShape *)hkThreadMemory_allocateChunk(hkThreadMemory_getCurrent(), 224,
                                                                      HK_MEMORY_CLASS_DESTRUCTION);
        compoundShape->base.m_memSizeAndFlags = 224;
        hkdCompoundBreakableShape_construct(compoundShape, 0, localShapes.m_data, localShapes.m_size,
                                            CTR_FLAGS_SET_CHILD_PARENT);

        hkdBreakableShape *compoundBreakableShape = (hkdBreakableShape *)compoundShape;
        if (shapeBlueprint)
            hkdBreakableShape_setSimpleValuesFromBlueprint(compoundBreakableShape, shapeBlueprint);
        if (compoundBreakableShape->m_strength == 0.0f)
        {
            compoundBreakableShape->m_strength = 1.0f;
            compoundShape->m_useChildrenBreakableThreshold.m_bool = 1;
        }

        if (shapeInstancesOut->m_size == (shapeInstancesOut->m_capacityAndFlags & 0x3FFFFFFF))
            hkArrayUtil__reserveMore(shapeInstancesOut, sizeof(hkdShapeInstanceInfo));

        {
            int outIndex = shapeInstancesOut->m_size;
            hkdShapeInstanceInfo *outData = shapeInstancesOut->m_data;

            defaultRuntimeInfo.m_distanceToDestructionPoint = 3.40282347e38f;
            defaultRuntimeInfo.m_forceFixed = 0;
            defaultRuntimeInfo.m_oldChildIdx = 0xFFFF;

            shapeInstancesOut->m_size = outIndex + 1;
            resultShape = compoundBreakableShape;

            outData[outIndex].m_shape = compoundBreakableShape;
            outData[outIndex].m_damage = 0;
            outData[outIndex].m_runtimeInfo = defaultRuntimeInfo;
            /* DEVIATION: trailing stvx128 v123..v126 store shapeInfo->m_transform
               (hoisted into VMX regs at the prologue) as the instance transform. */
            outData[outIndex].m_transform = shapeInfo->m_transform;
        }
    }
    else
    {
        /* SIMPLE path: one breakable shape built directly from the whole physics
           shape, adopting any accumulated children. */
        resultShape = createBreakableShapeFromBlueprint(shapeBlueprint, physicsShape, &fractureInfo,
                                                        containerGraphicsNode, graphicsSystem, isDeformable);
        if (!resultShape)
            goto cleanup;

        {
            int childIndex;
            for (childIndex = 0; childIndex < localShapes.m_size; ++childIndex)
                hkdBreakableShape_addChild(resultShape, localShapes.m_data[childIndex].m_shape,
                                           &localShapes.m_data[childIndex].m_transform);
        }
        setBreakableShapeFracture(&fractureInfo, resultShape, containerGraphicsNode,
                                  output->m_fractureTasks, isDeformable);
        if (!shapeBlueprint && localShapes.m_size)
            hkdBreakableShape_setSimpleValuesFromChildren(resultShape, localShapes.m_data, localShapes.m_size);

        defaultRuntimeInfo.m_forceFixed = 0;
        defaultRuntimeInfo.m_oldChildIdx = 0xFFFF;
        defaultRuntimeInfo.m_distanceToDestructionPoint = 3.40282347e38f;

        if (shapeInstancesOut->m_size == (shapeInstancesOut->m_capacityAndFlags & 0x3FFFFFFF))
            hkArrayUtil__reserveMore(shapeInstancesOut, sizeof(hkdShapeInstanceInfo));

        {
            int outIndex = shapeInstancesOut->m_size;
            hkdShapeInstanceInfo *outData = shapeInstancesOut->m_data;

            shapeInstancesOut->m_size = outIndex + 1;
            outData[outIndex].m_shape = resultShape;
            outData[outIndex].m_damage = 0;
            outData[outIndex].m_runtimeInfo = defaultRuntimeInfo;
            /* DEVIATION: shared trailing stvx128 store -- instance transform =
               shapeInfo->m_transform (VMX regs from the prologue). */
            outData[outIndex].m_transform = shapeInfo->m_transform;
        }
    }

search_shape_actions:
    /* Attach every action resource in this container to the built shape. */
    {
        const hkResourceHandle *actionHandle;
        for (actionHandle = hkResourceContainer_findResourceByName(container, 0, &hkdActionClass, 0);
             actionHandle;
             actionHandle = hkResourceContainer_findResourceByName(container, 0, &hkdActionClass,
                                                                  (void *)actionHandle))
        {
            hkdAction *action = (hkdAction *)hkResourceHandle_getObject((hkResourceHandle *)actionHandle);
            hkdBreakableShape_addAction(resultShape, action);
        }
    }

cleanup:
    /* Release our references to the accumulated child shapes and free the local
       array's backing store. */
    {
        int i;
        for (i = 0; i < localShapes.m_size; ++i)
            hkReferencedObject_removeReference((hkReferencedObject *)localShapes.m_data[i].m_shape);
    }
    if (localShapes.m_capacityAndFlags >= 0)
        hkThreadMemory_deallocateChunk(hkThreadMemory_getCurrent(), localShapes.m_data,
                                       /* = 80 * capacity (element size 80) when storage flags are clear */
                                       16 * ((localShapes.m_capacityAndFlags & 0x3FFFFFFF) +
                                             4 * localShapes.m_capacityAndFlags),
                                       HK_MEMORY_CLASS_ARRAY);
}
