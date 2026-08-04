#include "psSECTION_INTERFACE.h"
#include "psSECTION_DATA.h"
#include "../ds/dsSTRID.h"
// 0x82516944 — this section's own interned name; an empty-string dsSTRID when the handle is null.
// sret return: the decompiler swaps the sret slot with `this`.

extern const char psEmptyStringLiteral[]; // 0x8200155A — boundary

dsSTRID psSECTION_INTERFACE::GetName() const
{
    if (this->pData)
        return this->pData->name;
    return dsSTRID(psEmptyStringLiteral, 1);
}
