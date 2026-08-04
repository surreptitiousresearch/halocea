#include "../../headers/hcex/haloENGINE_CONTROL.h"
#include "../../headers/hcex/engine_control/haloENGINE_CONTROL_boundary.h"

// haloENGINE_CONTROL::CreateCheckpointList @ 0x823CA034 — every checkpoint file found under
// pathToCheckpoints/level[/coop] whose extension matches hcex_chpt_ext, as leaf names (plus a
// synthetic leading "_start" entry).
//
// DEVIATION: the DB's raw prototype mistypes the hidden sret result pointer as a second
// `haloENGINE_CONTROL*` parameter (classic PPC struct-return-by-value ABI artifact — see
// CLAUDE.md's decompiler gotchas and the sibling haloENGINE_CONTROL_ssl_bridge.h note). Real
// signature returns the list by value (sret), taking only `level`.
//
// CAVEAT: the decompiled tail on every path (`this->eventDispatcher.__vftable = (...)v16.m_pData;
// ...; ++v39->OnEvent;`) is register/stack-slot-reuse noise around the real
// `ds_data::REF_TYPE<dsREF_VECTOR>::Release(&v16, v16.m_pData)` call immediately after it — writing
// a REF_TYPE_DATA pointer into an unrelated vtable field and incrementing a function pointer field
// cannot be real observable behavior. Reproduced as the real Release() call only.
ds_data::REF_TYPE<dsREF_VECTOR> haloENGINE_CONTROL::CreateCheckpointList(const dsTSTRING<char> &level)
{
    ds_data::REF_TYPE<dsREF_VECTOR> result;

    if (!load_as_halo)
    {
        result.Release(result.m_pData);
        return result;
    }

    dsTSTRING<char> startMarker;
    startMarker.pBuffer = nullptr;
    startMarker.UnsafeInit("_start", -1, 0);

    dsDATA startValue;
    startValue.StoreValue<dsTSTRING<char>>(startMarker);
    result->Push(&startValue);

    if (--startMarker.pBuffer->refCount == 0)
        dlFree(startMarker.pBuffer);

    dsTSTRING<char> searchDir = pathToCheckpoints + level;
    if (IsCoop())
        searchDir.Insert(searchDir.pBuffer->strLen, "\\coop", -1);

    dsVECTOR<dsTSTRING<char>, 8> matches;
    matches.pData = nullptr;
    matches.nElem = 0;
    matches.allocated = 0;

    dsTSTRING<char> searchDirLower = searchDir.Lower();
    // Wrap the C-string operands in dsTSTRING<char> so the unambiguous (dsTSTRING,dsTSTRING)
    // operator+ is selected: two identical (dsTSTRING&, const char*) operator+ templates are
    // visible here (dsTSTRING.h and haloENGINE_CONTROL_boundary.h), which are otherwise ambiguous.
    dsTSTRING<char> maskPrefix = searchDirLower + dsTSTRING<char>("\\*.");
    dsTSTRING<char> mask = maskPrefix + dsTSTRING<char>(hcex_chpt_ext);
    if (--maskPrefix.pBuffer->refCount == 0)
        dlFree(maskPrefix.pBuffer);
    if (--searchDirLower.pBuffer->refCount == 0)
        dlFree(searchDirLower.pBuffer);

    dsTSTRING<char> maskCopy;
    maskCopy.pBuffer = nullptr;
    maskCopy.UnsafeInit(mask.pBuffer->str, -1, 0);

    fnmFILE_LOOK_THROUGH lookThrough;
    lookThrough.Find(maskCopy, false);

    int count = matches.nElem;
    for (int i = 0; i < count; ++i)
    {
        dsTSTRING<char> leafName;
        leafName.pBuffer = nullptr;
        fnmGetNameStr(&leafName, &matches[i]);

        dsDATA entryValue;
        entryValue.StoreValue<dsTSTRING<char>>(leafName);
        result->Push(&entryValue);

        if (--leafName.pBuffer->refCount == 0)
            dlFree(leafName.pBuffer);
    }

    if (--mask.pBuffer->refCount == 0)
        dlFree(mask.pBuffer);

    // `matches` and `searchDir` are released by their own destructors at scope exit (dsVECTOR<T,8>
    // and dsTSTRING<char> both declare one) rather than an explicit call here, matching this
    // corpus's established convention (see dsDATA.h's CAVEAT on this exact double-release wrinkle).

    result.Release(result.m_pData);
    return result;
}
