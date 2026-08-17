/* ?_SelectMaterialsByInstAffixes@MTL_DESC_DB@@AAAXPAVobjOBJ@@ABV?$dsTSTRING@D@@@Z @0x82A83680 */
#include "MTL_DESC_DB.h"
#include "../obj/objOBJ.h"
#include "../obj/objSPLIT.h"
#include "../vid/vidORD.h"
#include "../ds/dsAFFIX_STRING.h"
#include <cstring>

// MTL_DESC_DB::_SelectMaterialsByInstAffixes(objOBJ *, const dsTSTRING<char> &) — 0x82A83680
// ABI: r3 = this, r4 = pObj, r5 = instAffixes (no sret — void return).
//
// For a single object with geometry: resolve its combined affix string, extract an optional
// explicit material-name override from it, and compute a CRC32 suffix from the (sdr-filtered)
// affix string. Then walk every split entry of the object's shared geometry, look up that
// entry's own resolved material (mtlTBL::FindMtl keyed on tag index 0 -- see caveat below),
// build a full lookup name (entry material's own name, or "default" if empty, plus the CRC32
// suffix), and use the explicit override name instead when one was found. Whichever name wins
// is looked up in the corresponding vidORD split's material list, and if found becomes that
// split's current material.
//
// CAVEAT: objGEOM_UNSHARED::pRendData is declared rendORD* (the abstract/partial render-order
// base), but disasm confirms the accessed field (a dsVECTOR<vidOBJ_SPLIT,8> at pRendData+0x60)
// only exists on the derived vidORD (rendORD itself is 50 bytes; vidORD's splitList sits at
// offset 96/0x60 -- see src/headers/ws/vid/vidORD.h). Reproduced as a cast to vidORD*, matching
// what the shipped binary actually dereferences.
void MTL_DESC_DB::_SelectMaterialsByInstAffixes(objOBJ *pObj, const dsTSTRING<char> &instAffixes)
{
    if (!pObj->pGeom)
        return;

    dsTSTRING<char> affixString = this->BuildMtlAffixString(pObj, instAffixes);

    // Explicit "material_name" override extracted from a "vid.mtl"/"vid.mtl_strict" affix, if
    // present. Scoped: affixWrapper's buffer is released immediately after this call (disasm-
    // confirmed -- it is torn down before FilterAffixStringOverride even runs).
    dsTSTRING<char> overrideMaterialName;
    {
        dsAFFIX_STRING affixWrapper(affixString);
        overrideMaterialName = this->_GetMtlNameFromAffix(affixWrapper);
    }

    // CRC32 suffix computed from the affix string filtered down to "vid.sdr.*" affixes only.
    dsTSTRING<char> crcSuffix;
    {
        dsTSTRING<char> filteredOverrideAffixes = this->FilterAffixStringOverride(affixString);
        crcSuffix = this->_MtlSuffixFromAffixString(filteredOverrideAffixes);
    }

    objGEOM_UNSHARED *pGeom = pObj->pGeom;
    objGEOM_SHARED   *pSharedGeom = pGeom->pSharedGeom;
    vidORD           *pRendData = (vidORD *)pGeom->pRendData; // see CAVEAT above
    objSPLIT         *pSplit = pSharedGeom->pSplit;

    for (int splitIndex = 0; splitIndex < pSplit->nEntry; ++splitIndex)
    {
        objSPLIT_ENTRY *entry = pSplit->GetEntry(splitIndex);
        vidOBJ_SPLIT   *splitSlot = &pRendData->splitList[splitIndex];

        // NOTE: mtl==0, startPos==0, direction==0 are hardcoded at every call site in this
        // function (matches disasm literally -- not a placeholder).
        mtlMTL *entryMtl = entry->mtlTbl.FindMtl(0, 0, 0);
        if (!entryMtl)
            continue;

        dsTSTRING<char> entryMtlName;
        entryMtlName.pBuffer = nullptr;
        entryMtlName.UnsafeInit(entryMtl->param.___u0.base.mtlName, -1, 0); // mtlPARAM wraps anon union ___u0

        if (!entryMtlName.pBuffer->strLen)
        {
            dsTSTRING<char> defaultMtlName;
            defaultMtlName.pBuffer = nullptr;
            defaultMtlName.UnsafeInit("default", -1, 0);
            entryMtlName = defaultMtlName;
        }

        // Append the CRC32 suffix onto the entry's own material name (gap-open + raw copy,
        // matching mtlMTL_MNG::BuildMtlAffixString's established pattern for this codebase).
        // public Insert(pos, s, len) — equivalent to the protected gap-open + memcpy the decompiler
        // showed, which is inaccessible outside dsTSTRING.
        int entryMtlNameLen = entryMtlName.pBuffer->strLen;
        entryMtlName.Insert(entryMtlNameLen, crcSuffix.pBuffer->str, crcSuffix.pBuffer->strLen);

        // An explicit "material_name" affix override always wins over the computed
        // entry-name-plus-suffix.
        const dsTSTRING<char> &lookupName =
            overrideMaterialName.pBuffer->strLen ? overrideMaterialName : entryMtlName;

        int foundSlot = splitSlot->FindMtl(lookupName);
        if (foundSlot != -1)
            splitSlot->curMtl = (char)foundSlot;
    }
}
