#include "psSECTION_INTERFACE.h"
#include "../ds/dsSTRID.h"
// 0x82519480 — key name at record index `idx`, forwarding to the body; an empty (interned "")
// dsSTRID when the handle is null. sret return: the decompiler swaps the sret slot with `this`.

// 0x8200155A — interned empty-string literal used to build an empty dsSTRID. boundary.
extern const char psEmptyStringLiteral[];

dsSTRID psSECTION_INTERFACE::GetKeyName(int idx) const
{
    if (this->pData)
        return this->pData->GetKeyName(idx);
    return dsSTRID(psEmptyStringLiteral, 1);
}
