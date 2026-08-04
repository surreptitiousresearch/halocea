#include "../../headers/ws/ent/entSFX.h"
#include "../../headers/ws/ent/entSFX_COLOR.h"

// entSFX::IsA<entSFX_COLOR> @ 0x823EAD48 (??$IsA@VentSFX_COLOR@@@entSFX@@QBA_NXZ)
// Identical to IsA<entSFX_LIGHT> but tested against entSFX_COLOR_DESC::TYPE_ID. Target of the
// flattened extern entSFX_IsA_COLOR. See entSFX__IsA_LIGHT.cpp for the type-info-query deviation.
template<> int entSFX::IsA<entSFX_COLOR>() const
{
    dscDESC *desc = this->spDesc.pointee;
    const dsTYPE_ID *curType = (const dsTYPE_ID *)desc->__vftable->GetCurTypeInfo(desc);
    return curType->IsDerivedFrom(entSFX_COLOR_DESC::TYPE_ID) != 0;
}
