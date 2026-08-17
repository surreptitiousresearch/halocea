/* ?setSimpleValuesFromBlueprint@hkdBreakableShape@@QAAXPBVhkdShape@@@Z @0x83848B38 */
#include "../headers/havok/hkdBreakableShape.h"
#include "../headers/havok/hkdShape.h"

/* Deeper callee kept as a boundary extern (2-level descent limit). */
extern void hkdAssetProcessingUtil_convertAttributesToProperties(
    const hkArray<hkxAttribute> *attributes, const char *parentName, hkdProperties *propertiesOut);

/* hkdBreakableShape::setSimpleValuesFromBlueprint — copy the scalar/enum
   authoring fields from a shape blueprint (hkdShape) onto this breakable shape,
   and convert the blueprint's attribute list into runtime properties. */
void hkdBreakableShape_setSimpleValuesFromBlueprint(hkdBreakableShape *self, const hkdShape *shapeBp)
{
    if (!shapeBp->m_flattenChildCompounds.m_bool)
        self->m_flags |= FLAG_DO_NOT_FLATTEN_RECURSIVE_COMPOUNDS;
    self->m_strength                = shapeBp->m_strength;
    self->m_relativeSubpieceStrength = shapeBp->m_relativeSubpieceStrength;
    self->m_minDestructionRadius    = shapeBp->m_destructionRadius;
    self->m_breakingPropagationRate = shapeBp->m_breakingPropagationRate;
    self->m_qualityType.m_storage       = shapeBp->m_bodyQualityType.m_storage;
    self->m_connectivityType.m_storage  = shapeBp->m_connectivity.m_storage;
    self->m_flexibleJointType.m_storage = shapeBp->m_flexibleJointType.m_storage;
    self->m_integrityType.m_storage     = shapeBp->m_integrityType.m_storage;
    self->m_tensionLength           = shapeBp->m_tensionLength;
    hkdAssetProcessingUtil_convertAttributesToProperties(&shapeBp->m_attributes,
                                                         shapeBp->m_parentName,
                                                         &self->m_properties);
}
