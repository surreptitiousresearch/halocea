#include "../headers/havok/hkdBreakableBody.h"
#include "../headers/havok/hkdWorld.h"
#include "../headers/havok/hkdBreakableShape.h"
#include "../headers/havok/hkdGeometry.h"
#include "../headers/havok/hkdShapeInstanceInfo.h"
#include "../headers/havok/hkdBreakableBodyFactory_boundary.h"
#include "../headers/havok/hkThreadMemory.h"

typedef struct hkpShape hkpShape;

extern const hkTransform hkTransformIdentity;
extern const int HK_MEMORY_CLASS_DESTRUCTION;
extern void hkdBreakableShape_construct(hkdBreakableShape *self, const hkpShape *shape, void *unused, hkdGeometry *geom); /* hkdBreakableShape::hkdBreakableShape */
extern void hkdBreakableShape_setSimpleValuesFromParent(hkdBreakableShape *self, const hkdBreakableShape *parent);

/* Create a child breakable body whose shape is a fresh breakable shape wrapping
   shape/geom, inheriting the parent shape's simple values and dynamic fracture.
   The new shape is handed to the world breakable-body factory as a single
   identity-transformed instance; the factory retains its own reference, so ours
   is released before returning. Stack layout decoded from disasm 0x8385E658. */
hkdBreakableBody *SphereGun_newBreakableBody(hkdWorld *destructionWorld, const hkdBreakableBody *parentBody,
                                             const hkpShape *shape, hkdGeometry *geom)
{
    const hkdBreakableShape *parentShape = parentBody->m_breakableShape.m_pntr;
    hkdBreakableShape *newShape;
    hkdBreakableBodyFactory *factory;
    hkdController *controller;
    hkdShapeInstanceInfo instance;
    hkdBreakableBodyFactory_CreateBodyInput input;
    hkdBreakableBody *result;

    newShape = (hkdBreakableShape *)hkThreadMemory_allocateChunk(hkThreadMemory_getCurrent(), 208, HK_MEMORY_CLASS_DESTRUCTION);
    newShape->m_memSizeAndFlags = 208;
    hkdBreakableShape_construct(newShape, shape, 0, geom);
    hkdBreakableShape_setSimpleValuesFromParent(newShape, parentShape);

    /* inherit the parent dynamic fracture (ref the incoming, unref the old) */
    if (parentShape->m_dynamicFracture.m_pntr)
        hkReferencedObject_addReference((hkReferencedObject *)parentShape->m_dynamicFracture.m_pntr);
    if (newShape->m_dynamicFracture.m_pntr)
        hkReferencedObject_removeReference((hkReferencedObject *)newShape->m_dynamicFracture.m_pntr);
    newShape->m_dynamicFracture.m_pntr = parentShape->m_dynamicFracture.m_pntr;

    factory = destructionWorld->m_breakableBodyFactory.m_pntr;
    controller = parentBody->m_controller.m_pntr;

    /* one shape instance: identity transform + the new shape, default runtime info */
    instance.m_transform = hkTransformIdentity;
    instance.m_shape = newShape;
    instance.m_damage = 0;
    instance.m_runtimeInfo.m_distanceToDestructionPoint = 3.40282e38f; /* FLT_MAX */
    instance.m_runtimeInfo.m_oldChildIdx = 0xFFFF;
    instance.m_runtimeInfo.m_forceFixed = 0;

    /* create-body input. Only col0.x of the parent transform is written (-2.0
       sentinel); the rest is intentionally left uninitialized -- the factory
       inherits the transform from m_parentBreakableBody. */
    input.m_parentTransform.m_rotation.m_col0.m_quad.___u0.v[0] = -2.0f;
    input.m_parentBreakableBody = parentBody;
    input.m_templateInfo = 0;
    input.m_useBreakableShapeMassProperties.m_bool = 1;
    input.m_controller = controller;

    result = hkdBreakableBodyFactory_createBreakableBody(factory, &instance, &input);
    hkReferencedObject_removeReference((hkReferencedObject *)newShape);
    return result;
}
