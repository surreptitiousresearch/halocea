#include "psITERATOR.h"
#include "psSECTION.h"
#include "../ds/dsDATA.h"
// 0x8251A648 (?GetSection@psITERATOR@@...) — the current record's value coerced to a section
// handle, written into `*result` (empty when the current value is not a section). sret return.
// The decompiler mislabels the sret `result` as `this` and the dsDATA temp as a psITERATOR (a
// stack-slot-reuse artifact); the disasm shows the real order: sret in r3, `this` in r4.

// ?GetValue@psITERATOR@@QBA?AVdsDATA@@XZ — the current record's boxed value (sret). boundary.
extern int psDataToSection(const dsDATA &data, psSECTION *out); // boundary — dsDATA -> psSECTION

psSECTION *psITERATOR::GetSection(psSECTION *result) const
{
    result->pData = nullptr;
    dsDATA value = this->GetValue();
    psDataToSection(value, result);
    if (value.type)
        value.type->Destroy(&value.storage);
    return result;
}
