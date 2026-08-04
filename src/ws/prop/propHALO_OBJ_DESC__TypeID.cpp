#include "../../headers/ws/prop/propHALO_OBJ_DESC.h"

// propHALO_OBJ_DESC::TypeID() const @ 0x823EB2C8
// DEVIATION: the decompiler rendered this as writing __vftable/refCount/pBrand/isUnshared
// fields on `this`, but the disassembly (four lwz from propHALO_OBJ_DESC::TYPE_ID.id.array
// followed by four stw to 0(r3)/4(r3)/8(r3)/0xC(r3), with r3 the sret result pointer and `this`
// register never touched) shows this is a plain 16-byte copy of the static class-wide TYPE_ID
// bitmask into the caller-supplied result — `this` is unused. The DB's nominal (this, result)
// parameter order is the register truth here (r3 = the hidden sret pointer for the by-value
// dsTYPE_ID return, matching the propBASE-style sret convention documented in dscDESC_vtbl).
dsTYPE_ID propHALO_OBJ_DESC::TypeID() const
{
    return propHALO_OBJ_DESC::TYPE_ID;
}
