#include "../../headers/ws/ent/entSFX.h"
#include "../../headers/ws/ent/entSFX_LIGHT.h"

// entSFX::IsA<entSFX_LIGHT> @ 0x823D1E08 (??$IsA@VentSFX_LIGHT@@@entSFX@@QBA_NXZ)
// True when this sfx's descriptor's current runtime type derives from entSFX_LIGHT_DESC.
//
// DEVIATION: the decompiler renders the type-info query as `spDesc.pointee->__vftable[1]
// .GetCurTypeInfo(&scratch)` (an sret virtual call), casting the result to dsTYPE_ID*. It is
// reproduced here as the modeled slot-0 virtual GetCurTypeInfo, whose dscTYPE_INFO result begins
// with the dsTYPE_ID that IsDerivedFrom tests. Target of the flattened extern entSFX_IsA_LIGHT.
template<> int entSFX::IsA<entSFX_LIGHT>() const
{
    dscDESC *desc = this->spDesc.pointee;
    const dsTYPE_ID *curType = (const dsTYPE_ID *)desc->__vftable->GetCurTypeInfo(desc);
    return curType->IsDerivedFrom(entSFX_LIGHT_DESC::TYPE_ID) != 0;
}
