#include "../headers/havok/hkResourceContainer.h"
#include "../headers/havok/hkpWorldObject.h"
#include "../headers/havok/hkTransform.h"
#include "../headers/havok/hkError.h"

/* hkTransform::setMulInverseMul — out = inverse(a) * b (Havok SDK boundary). */
extern void hkTransform_setMulInverseMul(hkTransform *out, const hkTransform *a,
                                         const hkTransform *b);

/* Offset of the local hkTransform inside an hkpShapeInfo object. */
#define HKP_SHAPE_INFO_TRANSFORM_OFFSET 0x30

/* Recursively re-express a resource container's geometry in a new parent frame.
   - A rigid body cannot be retransformed: warn and stop that branch.
   - A shape info's transform is rebased into the parent frame
     (localTransform = inverse(parentTransform) * localTransform).
   - Otherwise recurse into every child container. */
void retransformResourceTree(hkResourceContainer *container, const hkTransform *transform)
{
    hkResourceHandle *rigidBodyHandle;
    void *rigidBody;

    rigidBodyHandle = hkResourceContainer_findResourceByName(container, 0, &hkpRigidBodyClass, 0);
    if (rigidBodyHandle && (rigidBody = hkResourceHandle_getObject(rigidBodyHandle)) != 0)
    {
        const char *bodyName = ((const hkpWorldObject *)rigidBody)->m_name; /* +0x74, DB hkpWorldObject.m_name */
        hkErrStream stream;
        char buffer[512];

        hkErrStream_construct(&stream, buffer, 512);
        hkOstream_insertString((hkOstream *)&stream, "Cannot retransform a rigidbody: '");
        hkOstream_insertString((hkOstream *)&stream, bodyName);
        hkOstream_insertString((hkOstream *)&stream, "'");
        hkError_message(hkError_s_instance, MESSAGE_WARNING, 0xABBA3434, buffer,
                        "D:\\Projects\\code\\common\\lib_3dpart\\phys\\hk_include\\Destruction\\Destruction\\AssetProcessing\\hkdAssetProcessingUtil.cpp",
                        425);
        hkOstream_destruct((hkOstream *)&stream);
    }
    else
    {
        hkResourceHandle *shapeInfoHandle;
        void *shapeInfo;

        shapeInfoHandle = hkResourceContainer_findResourceByName(container, 0, &hkpShapeInfoClass, 0);
        if (shapeInfoHandle && (shapeInfo = hkResourceHandle_getObject(shapeInfoHandle)) != 0)
        {
            hkTransform *shapeTransform =
                (hkTransform *)((char *)shapeInfo + HKP_SHAPE_INFO_TRANSFORM_OFFSET);
            hkTransform localTransform;

            /* The lvx128/stvx128 block in the disassembly is a plain 64-byte
               hkTransform copy of the setMulInverseMul result back into the
               shape info's transform slot. */
            hkTransform_setMulInverseMul(&localTransform, transform, shapeTransform);
            *shapeTransform = localTransform;
        }
        else
        {
            hkResourceContainer *child;

            for (child = hkResourceContainer_findContainerByName(container, 0, 0);
                 child;
                 child = hkResourceContainer_findContainerByName(container, 0, child))
            {
                retransformResourceTree(child, transform);
            }
        }
    }
}
