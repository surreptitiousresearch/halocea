// FUNCTION_INDEX entry: hkdBreakableShape_construct @0x83849EF8 (??0hkdBreakableShape@@QAA@PBVhkpShape@@PBUhkpMassProperties@@PBVhkdGeometry@@@Z)
#include "../headers/havok/hkdBreakableShape.h"
#include "../headers/havok/hkReferencedObject.h"
#include <string.h>

/* boundary — vtable symbol and the inertia-tensor helper (level-2 callee). */
extern void *const hkdBreakableShape_vftable; /* hkdBreakableShape::`vftable' */
extern void hkpInertiaTensorComputer_computeShapeVolumeMassProperties(
    const hkpShape *shape, float density, hkpMassProperties *out); /* hkpInertiaTensorComputer::computeShapeVolumeMassProperties */

/* hkdBreakableShape::hkdBreakableShape — build a simple breakable shape over a
   physics shape (referenced) and optional source geometry (referenced), with
   empty child/connection/property/action arrays and default fracture tuning
   (breakingPropagationRate 0.1, relativeSubpieceStrength 1.0). When mass
   properties are supplied they are copied in; otherwise they are computed from
   the physics shape at unit density (or left zeroed when there is no shape).
   DEVIATION: empty hkArrays are stamped with the DONT_DEALLOCATE flag
   0x80000000 as the original does; VMX128 zeroing of the center-of-mass /
   inertia tensor is reconstructed with memset. */
void hkdBreakableShape_construct(hkdBreakableShape *self, const hkpShape *physicsShape,
                                 const hkpMassProperties *massProps, const hkdGeometry *geometry)
{
    self->m_referenceCount = 1;
    self->__vftable = (hkBaseObject_vtbl *)&hkdBreakableShape_vftable;
    self->m_type.m_storage = SHAPE_TYPE_SIMPLE;
    self->m_parent = 0;

    self->m_children.m_data = 0;
    self->m_children.m_size = 0;
    self->m_children.m_capacityAndFlags = 0x80000000;
    self->m_connections.m_data = 0;
    self->m_connections.m_size = 0;
    self->m_connections.m_capacityAndFlags = 0x80000000;

    if (physicsShape)
        hkReferencedObject_addReference((hkReferencedObject *)physicsShape);
    self->m_physicsShape.m_pntr = physicsShape;
    if (geometry)
        hkReferencedObject_addReference((hkReferencedObject *)geometry);
    self->m_geometry.m_pntr = geometry;

    self->m_graphicsShape.m_pntr = 0;
    self->m_graphicsShapeName = 0;
    self->m_properties.m_properties.m_data = 0;
    self->m_properties.m_properties.m_size = 0;
    self->m_properties.m_properties.m_capacityAndFlags = 0x80000000;
    self->m_actions.m_data = 0;
    self->m_actions.m_size = 0;
    self->m_actions.m_capacityAndFlags = 0x80000000;
    self->m_dynamicFracture.m_pntr = 0;

    self->m_referenceShapeVolume = 0.0f;
    self->m_massProps.m_volume = 0.0f;
    self->m_massProps.m_mass = 0.0f;
    memset(&self->m_massProps.m_centerOfMass, 0, sizeof(self->m_massProps.m_centerOfMass));
    memset(&self->m_massProps.m_inertiaTensor, 0, sizeof(self->m_massProps.m_inertiaTensor));

    self->m_flags = 0;
    self->m_connectivityType.m_storage = 0;
    self->m_flexibleJointType.m_storage = 0;
    self->m_strength = 0.0f;
    self->m_qualityType.m_storage = 0;
    self->m_relativeSubpieceStrength = 1.0f;
    self->m_integrityType.m_storage = 0;
    self->m_tensionLength = 0.0f;
    self->m_breakingPropagationRate = 0.1f;
    self->m_minDestructionRadius = 0.0f;

    if (massProps)
    {
        self->m_massProps.m_volume = massProps->m_volume;
        self->m_massProps.m_mass = massProps->m_mass;
        self->m_massProps.m_centerOfMass = massProps->m_centerOfMass;
        self->m_massProps.m_inertiaTensor = massProps->m_inertiaTensor;
    }
    else if (self->m_physicsShape.m_pntr)
    {
        hkpInertiaTensorComputer_computeShapeVolumeMassProperties(
            self->m_physicsShape.m_pntr, 1.0f, &self->m_massProps);
    }
}
