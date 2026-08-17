/* getTransformOfNode @0x83839310 */
#include "../headers/havok/hkResourceContainer.h"
#include "../headers/havok/hkTransform.h"

/* hkTransform::setMul — out = a * b (Havok SDK boundary, vtable-free method). */
extern void hkTransform_setMul(hkTransform *out, const hkTransform *a, const hkTransform *b);

/* Byte offsets of the local hkTransform inside the two resource object types. */
#define HKP_SHAPE_INFO_TRANSFORM_OFFSET 0x30
#define HKP_RIGID_BODY_TRANSFORM_OFFSET 0xF0

/* Resolve the world transform of a resource container's node.
   - A rigid body carries an absolute transform (offset 0xF0): use it directly.
   - Otherwise start from identity, override with the shape info's local
     transform (offset 0x30) if present, then compose up the parent chain:
     world = parentWorld * local.  The lvx128/stvx128 blocks in the
     disassembly are plain 64-byte hkTransform copies. */
void getTransformOfNode(hkResourceContainer *container, hkTransform *transformOut)
{
    hkResourceHandle *shapeInfoHandle;
    hkResourceHandle *rigidBodyHandle;
    void *shapeInfo = 0;
    void *rigidBody;

    shapeInfoHandle = hkResourceContainer_findResourceByName(container, 0, &hkpShapeInfoClass, 0);
    if (shapeInfoHandle)
        shapeInfo = hkResourceHandle_getObject(shapeInfoHandle);

    rigidBodyHandle = hkResourceContainer_findResourceByName(container, 0, &hkpRigidBodyClass, 0);
    if (rigidBodyHandle && (rigidBody = hkResourceHandle_getObject(rigidBodyHandle)) != 0)
    {
        *transformOut = *(const hkTransform *)((char *)rigidBody + HKP_RIGID_BODY_TRANSFORM_OFFSET);
        return;
    }

    {
        hkTransform localTransform;
        hkResourceContainer *parent;

        /* Identity transform. */
        localTransform.m_rotation.m_col0.m_quad.___u0.v[0] = 0.0f;
        localTransform.m_rotation.m_col0.m_quad.___u0.v[1] = 0.0f;
        localTransform.m_rotation.m_col0.m_quad.___u0.v[2] = 0.0f;
        localTransform.m_rotation.m_col0.m_quad.___u0.v[3] = 0.0f;
        localTransform.m_rotation.m_col1.m_quad.___u0.v[0] = 0.0f;
        localTransform.m_rotation.m_col1.m_quad.___u0.v[1] = 0.0f;
        localTransform.m_rotation.m_col1.m_quad.___u0.v[2] = 0.0f;
        localTransform.m_rotation.m_col1.m_quad.___u0.v[3] = 0.0f;
        localTransform.m_rotation.m_col2.m_quad.___u0.v[0] = 0.0f;
        localTransform.m_rotation.m_col2.m_quad.___u0.v[1] = 0.0f;
        localTransform.m_rotation.m_col2.m_quad.___u0.v[2] = 0.0f;
        localTransform.m_rotation.m_col2.m_quad.___u0.v[3] = 0.0f;
        localTransform.m_translation.m_quad.___u0.v[0] = 0.0f;
        localTransform.m_translation.m_quad.___u0.v[1] = 0.0f;
        localTransform.m_translation.m_quad.___u0.v[2] = 0.0f;
        localTransform.m_translation.m_quad.___u0.v[3] = 0.0f;
        localTransform.m_rotation.m_col0.m_quad.___u0.v[0] = 1.0f;
        localTransform.m_rotation.m_col1.m_quad.___u0.v[1] = 1.0f;
        localTransform.m_rotation.m_col2.m_quad.___u0.v[2] = 1.0f;

        if (shapeInfo)
            localTransform = *(const hkTransform *)((char *)shapeInfo + HKP_SHAPE_INFO_TRANSFORM_OFFSET);

        if (hkResourceContainer_getParent(container))
        {
            hkTransform parentTransform;

            parent = hkResourceContainer_getParent(container);
            getTransformOfNode(parent, &parentTransform);
            hkTransform_setMul(transformOut, &parentTransform, &localTransform);
        }
        else
        {
            *transformOut = localTransform;
        }
    }
}
