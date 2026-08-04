#ifndef HAVOK_HKPWORLDOBJECT_BROADPHASETYPE_H
#define HAVOK_HKPWORLDOBJECT_BROADPHASETYPE_H

/* hkpWorldObject::BroadPhaseType — classifies what kind of world object owns a
   broad-phase handle (hkpTypedBroadPhaseHandle::m_type).
   DB enum: types_enum_values type_name "hkpWorldObject::BroadPhaseType". */
enum hkpWorldObject_BroadPhaseType
{
    BROAD_PHASE_INVALID = 0,
    BROAD_PHASE_ENTITY  = 1,
    BROAD_PHASE_PHANTOM = 2,
    BROAD_PHASE_BORDER  = 3,
    BROAD_PHASE_MAX_ID  = 4
};

#endif /* HAVOK_HKPWORLDOBJECT_BROADPHASETYPE_H */
