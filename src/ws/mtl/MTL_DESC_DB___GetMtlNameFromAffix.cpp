/* ?_GetMtlNameFromAffix@MTL_DESC_DB@@AAA?AV?$dsTSTRING@D@@ABVdsAFFIX_STRING@@@Z @0x82A82BA0 */
#include "MTL_DESC_DB.h"
#include "../ds/dsAFFIX_STRING.h"
#include "../ds/dsAFFIX_READER.h"

// MTL_DESC_DB::_GetMtlNameFromAffix(const dsAFFIX_STRING &) — 0x82A82BA0
// ABI: r3 = sret result buffer (materialName, mistyped as MTL_DESC_DB* by the decompiler --
// disasm confirms r3/r4 are swapped from the decompiler's printed parameter list: r4 is the real
// `this`, loaded but never read by the body, matching every other mtlMTL_MNG-family sret method
// in this batch), r4 = this, r5 = affixes.
//
// Resolve a base material name from an affix string:
//   - "vid.mtl_strict" affix present: take its "material_name" parameter verbatim (empty string
//     if the parameter is missing -- no fallback to "vid.mtl").
//   - else "vid.mtl" affix present: take its "material_name" parameter, then append a CRC32
//     suffix computed from the affix string filtered down to "vid.sdr.*" affixes (empty string
//     if the parameter is missing).
//   - neither present: empty string.
dsTSTRING<char> MTL_DESC_DB::_GetMtlNameFromAffix(const dsAFFIX_STRING &affixes)
{
    dsTSTRING<char> materialName;
    materialName.pBuffer = nullptr;
    materialName.UnsafeInitEmpty();

    dsAFFIX_READER reader(affixes.str);

    if (reader.HaveAffix("vid.mtl_strict", false))
    {
        if (!reader.GetString("material_name", &materialName))
        {
            // Reset to the shared empty string (disasm shows a full re-adopt via a temporary +
            // operator=, not just leaving materialName as-is -- reproduced faithfully in case
            // GetString left a partial write on failure).
            dsTSTRING<char> emptyReset;
            emptyReset.pBuffer = nullptr;
            emptyReset.UnsafeInitEmpty();
            materialName = emptyReset;
        }
    }
    else if (reader.HaveAffix("vid.mtl", false))
    {
        if (reader.GetString("material_name", &materialName))
        {
            dsTSTRING<char> filteredOverrideAffixes = this->FilterAffixStringOverride(affixes.str);
            dsTSTRING<char> crcSuffix = this->_MtlSuffixFromAffixString(filteredOverrideAffixes);
            materialName += crcSuffix;
        }
        else
        {
            dsTSTRING<char> emptyReset;
            emptyReset.pBuffer = nullptr;
            emptyReset.UnsafeInitEmpty();
            materialName = emptyReset;
        }
    }

    return materialName;
}
