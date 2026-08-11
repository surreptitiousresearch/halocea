// haloENGINE_CONTROL__Init @0x823CCAE8
#include "../../headers/hcex/haloENGINE_CONTROL.h"
#include "../../headers/hcex/engine_control/haloENGINE_CONTROL_boundary.h"

// haloENGINE_CONTROL::Init @ 0x823CCB00 — one-time bring-up: register the "haloEngine" SSL
// class/object (via SSL_EXPORTER_haloENGINE_CONTROL), wire the input system's per-frame update
// callback, and read pathToCheckpoints from config (defaulting to "d:\checkpoints",
// trailing-slash-normalized). Returns false if pathToCheckpoints comes back empty.
//
// DEVIATION: the DB's raw prototype/decompile scrambles the (this, result, args) ordering across
// every sret call here (sslSYSTEM::AddClass/AddObject, both taking a hidden result pointer) —
// classic PPC struct-return-by-value ABI confusion (see CLAUDE.md's decompiler gotchas). Each
// sslCLASS_REF local the decompiler shows as `v9[1].pClass = (sslCLASS*)v9;` immediately before its
// own ctor call is that ctor's stack-alignment padding slot, not meaningful data — omitted here.
// Reconstructed in true register order: an empty root class ref, one AddClass() off it for the
// "haloEngine" class, then three independent copies of that class ref consumed by PerformExport and
// AddObject respectively.
bool haloENGINE_CONTROL::Init()
{
    sslCLASS_REF rootClass;
    sslCLASS_REF haloClass = gsSslSystem->AddClass(rootClass);

    {
        sslCLASS_REF exportClass(haloClass);
        SSL_EXPORTER_haloENGINE_CONTROL::Instance()->PerformExport(&exportClass);
    }

    {
        sslCLASS_REF objectClass(haloClass);
        sslOBJ_REF newObj = gsSslSystem->AddObject("haloEngine", objectClass, (sslCB_HOST *)this);
        sslObject = newObj;
    }

    gsSysInput->SetOnUpdateCB(OnInputUpdateCB);

    dsTSTRING<char> defaultPath;
    defaultPath.pBuffer = nullptr;
    defaultPath.UnsafeInit("d:\\checkpoints", -1, 0);
    // apCONFIG::Retrieve's DB ABI is (section, key, val&, defVal&) — the decompiler packed the two
    // key halves into one string and the refs as pointers; split/deref to match the header.
    gCfg->Retrieve<dsTSTRING<char>, dsTSTRING<char>>("Level", "PathCHPT", pathToCheckpoints, defaultPath);
    if (--defaultPath.pBuffer->refCount == 0)
        dlFree(defaultPath.pBuffer);

    if (!pathToCheckpoints.pBuffer->strLen)
        return false;

    int lastIdx = pathToCheckpoints.pBuffer->strLen - 1;
    if (pathToCheckpoints.pBuffer->str[lastIdx] != '\\')
        pathToCheckpoints.Insert(pathToCheckpoints.pBuffer->strLen, "\\", -1);

    return true;
}
