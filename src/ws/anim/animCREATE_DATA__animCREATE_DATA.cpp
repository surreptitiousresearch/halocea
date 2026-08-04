#include "animCREATE_DATA.h"

// animCREATE_DATA::animCREATE_DATA() @ 0x8252DDE0
// Wire up the vtable, clear srData, then delegate the remaining default field values (identity
// transform, scale 1, empty name, no sequence) to Clear(). ps/affixes are default-constructed
// implicitly as members (the decompile's explicit psSECTION::psSECTION()/dsAFFIX_STRING::
// dsAFFIX_STRING() calls are the compiler's ordinary member construction, not written out here).
animCREATE_DATA::animCREATE_DATA()
{
    this->__vftable = (animCREATE_DATA_vtbl *)&animCREATE_DATA::vftable;
    this->srData = nullptr;
    animCREATE_DATA::Clear();
}
