#include "../headers/havok/FractureInfo.h"
#include "../headers/havok/hkdBreakableShape.h"
#include "../headers/havok/hkdFracture.h"
#include "../headers/havok/hkdAssetProcessingUtil_GraphicsNode.h"
#include "../headers/havok/hkdAssetProcessingUtil_FractureTasks.h"
#include "../headers/havok/hkArray.h"
#include "../headers/havok/hkError.h"
#include "../headers/havok/hkReferencedObject.h"

/* Source file baked into every hkError::message call below. */
#define HKD_ASSET_PROCESSING_UTIL_CPP \
    "D:\\Projects\\code\\common\\lib_3dpart\\phys\\hk_include\\Destruction\\Destruction\\AssetProcessing\\hkdAssetProcessingUtil.cpp"

/* hkReferencedObject reference counting — Havok SDK boundary, not reversed. */
extern void hkReferencedObject_addReference(hkReferencedObject *self);    /* hkReferencedObject::addReference */
extern void hkReferencedObject_removeReference(hkReferencedObject *self); /* hkReferencedObject::removeReference */

/* Grow an hkArray so it has room for at least one more element of the given
   element size (12 = sizeof hkdAssetProcessingUtil::FractureTasks). Boundary. */
extern void hkArrayUtil__reserveMore(void *array, int elementSize); /* hkArrayUtil::_reserveMore */

/* Apply a graphics node's fracture info (from a shape blueprint) to its built
   hkdBreakableShape. The dynamic fracture is assigned by hkRefPtr semantics
   (add-ref the new one, release any previous), but only onto a valid, closed,
   leaf shape. The static fracture is not applied here; instead a fracture task
   is queued into fractureTasksOut for later processing, again only for a valid
   closed leaf shape. Deformable shapes are allowed to fracture without closed
   geometry, but then only the physics is fractured (a remark is emitted). */
void setBreakableShapeFracture(
    const FractureInfo *fractureInfo,
    hkdBreakableShape *breakableShape,
    const hkdAssetProcessingUtil_GraphicsNode *graphicsNode,
    hkArray<> *fractureTasksOut,
    unsigned char isDeformable)
{
    hkdFracture *dynamicFracture = fractureInfo->m_dynamicFracture;

    if (dynamicFracture)
    {
        if (!breakableShape->m_geometry.m_pntr)
        {
            hkErrStream stream;
            char buffer[592];

            hkErrStream_construct(&stream, buffer, 512);
            hkOstream_insertString((hkOstream *)&stream, "Your shape: '");
            hkOstream_insertString((hkOstream *)&stream, graphicsNode->m_name);
            hkOstream_insertString((hkOstream *)&stream,
                "' does not have a valid closed geometry, ignoring dynamic fracture");
            hkError_message(hkError_s_instance, MESSAGE_ERROR, 0xABBA3424, buffer,
                            HKD_ASSET_PROCESSING_UTIL_CPP, 937);
            hkOstream_destruct((hkOstream *)&stream);
        }
        else if (breakableShape->m_children.m_size)
        {
            hkErrStream stream;
            char buffer[592];

            hkErrStream_construct(&stream, buffer, 512);
            hkOstream_insertString((hkOstream *)&stream,
                "You cannot attach dynamic fracture to non leave shapes: '");
            hkOstream_insertString((hkOstream *)&stream, graphicsNode->m_name);
            hkOstream_insertString((hkOstream *)&stream, "'");
            hkError_message(hkError_s_instance, MESSAGE_WARNING, 0xABBA3DE7, buffer,
                            HKD_ASSET_PROCESSING_UTIL_CPP, 950);
            hkOstream_destruct((hkOstream *)&stream);
        }
        else if (!breakableShape->m_dynamicFracture.m_pntr)
        {
            /* hkRefPtr assignment inlined: add-ref new, release previous
               (previous is null on this branch), then store. */
            hkReferencedObject_addReference((hkReferencedObject *)dynamicFracture);
            if (breakableShape->m_dynamicFracture.m_pntr)
                hkReferencedObject_removeReference((hkReferencedObject *)breakableShape->m_dynamicFracture.m_pntr);
            breakableShape->m_dynamicFracture.m_pntr = dynamicFracture;
        }
    }

    if (!fractureInfo->m_fracture)
        return;

    if (!breakableShape->m_geometry.m_pntr)
    {
        hkErrStream stream;
        char buffer[592];

        if (!isDeformable)
        {
            hkErrStream_construct(&stream, buffer, 512);
            hkOstream_insertString((hkOstream *)&stream, "Your shape: '");
            hkOstream_insertString((hkOstream *)&stream, graphicsNode->m_name);
            hkOstream_insertString((hkOstream *)&stream,
                "' does not have a valid closed geometry, ignoring fracture");
            hkError_message(hkError_s_instance, MESSAGE_ERROR, 0xABBA3424, buffer,
                            HKD_ASSET_PROCESSING_UTIL_CPP, 961);
            hkOstream_destruct((hkOstream *)&stream);
            return;
        }

        hkErrStream_construct(&stream, buffer, 512);
        hkOstream_insertString((hkOstream *)&stream, "Remark [0xf03423de]: Object '");
        hkOstream_insertString((hkOstream *)&stream, graphicsNode->m_name);
        hkOstream_insertString((hkOstream *)&stream,
            "' is deformable: only the physics will be fractured. So reading back the data into the modelers is not possible.");
        hkError_message(hkError_s_instance, MESSAGE_REPORT, -1, buffer,
                        HKD_ASSET_PROCESSING_UTIL_CPP, 966);
        hkOstream_destruct((hkOstream *)&stream);
    }

    if (breakableShape->m_children.m_size)
    {
        hkErrStream stream;
        char buffer[592];

        hkErrStream_construct(&stream, buffer, 512);
        hkOstream_insertString((hkOstream *)&stream, "Your shape: '");
        hkOstream_insertString((hkOstream *)&stream, graphicsNode->m_name);
        hkOstream_insertString((hkOstream *)&stream,
            "' has children already, ignoring fracture");
        hkError_message(hkError_s_instance, MESSAGE_WARNING, 0xABBAFD23, buffer,
                        HKD_ASSET_PROCESSING_UTIL_CPP, 979);
        hkOstream_destruct((hkOstream *)&stream);
    }
    else
    {
        const char *nodeName = graphicsNode->m_name;
        hkdFracture *staticFracture = fractureInfo->m_fracture;
        hkdAssetProcessingUtil_FractureTasks *task;

        if (fractureTasksOut->m_size == (fractureTasksOut->m_capacityAndFlags & 0x3FFFFFFF))
            hkArrayUtil__reserveMore(fractureTasksOut, 12);

        task = &((hkdAssetProcessingUtil_FractureTasks *)fractureTasksOut->m_data)[fractureTasksOut->m_size];
        task->m_name = nodeName;
        task->m_shape = breakableShape;
        task->m_fracture = staticFracture;
        ++fractureTasksOut->m_size;
    }
}
