// aiBASS::Init @ 0x83242AE8
// ?Init@aiBASS@@MAA_NXZ
#include "../../headers/ws/ai/aiBASS.h"
#include "../../headers/ws/ps/psSECTION.h"
#include "../../headers/ws/ps/psSECTION_INTERFACE.h"
#include "../../headers/ws/ssl/sslSYSTEM.h"
#include "../../headers/ws/ssl/sslCLASS_REF.h"
#include "../../headers/ws/ssl/sslERROR.h"
#include "../../headers/ws/ds/dsDATA.h"

// txm_boundary shim: the shared psSYSTEM.h models the global `psSystem` as `psSECTION` (kept that
// way for the txm consumer). This TU needs the psSYSTEM* view + LoadPsFromFile, so it does NOT
// include psSYSTEM.h; it declares the global locally and calls it through the established
// psSYSTEM_LoadPsFromFile boundary shim (same pattern as aiPLANNER::ProcessINIT).
struct psSYSTEM;                           // ps — parametric-script system (pointer only here)
extern psSYSTEM *psSystem;                 // ?psSystem@@3PAVpsSYSTEM@@A — global ps-script system
// ?LoadPsFromFile@psSYSTEM@@QAA?AVpsSECTION@@V?$dsTSTRING@D@@00H@Z (via boundary shim) — read
// `fileName`, parse it, return the root section by value.
extern psSECTION psSYSTEM_LoadPsFromFile(psSYSTEM *sys, dsTSTRING<char> fileName,
                                         dsTSTRING<char> name, dsTSTRING<char> errHint,
                                         int ignorePreload);
extern sslSYSTEM *gsSslSystem;             // ?gsSslSystem@@3PAVsslSYSTEM@@A — global SSL system
extern int psDataToSection(const dsDATA &kv, psSECTION *out); // ?psDataToSection@@YAHABVdsDATA@@PAVpsSECTION@@@Z
extern void _apLog(const char *fmt, ...);  // ?_apLog@@YAXPBDZZ — engine logger  boundary

// One-time init: load "bass.cls", parse the BASS tuning + exclamation table out of it, register the
// SSL class and its native func callbacks, compile the class's embedded "#ssl" script, then create
// and name the scripted-sound object this system talks to.
//
// DEVIATION: the source decompile aliases the psSYSTEM load buffer, the derived config section, and
// the returned root section onto overlapping stack slots; the receiver objects below (root vs.
// section for GetStr) are reconstructed from the mangled callee signatures and the surrounding data
// flow rather than the raw slot names.
bool aiBASS::Init()
{
    psSECTION root = psSYSTEM_LoadPsFromFile(
        psSystem, dsTSTRING<char>("bass.cls"), dsTSTRING<char>("root"), dsTSTRING<char>(), 0);

    psSECTION_INTERFACE &rootIf = (psSECTION_INTERFACE &)root;
    if (rootIf.IsNull() || rootIf.IsEmpty())
        return false;

    psSECTION section;
    psDataToSection(rootIf.GetKeyValue(0), &section);

    dsTSTRING<char> hintErr;
    if (!ParsePS(section, hintErr))
        _apLog("~BASS,Error~aiBASS::Init(): failed to parse PS: %s", hintErr.CStr());

    sslCLASS_REF cls = gsSslSystem->AddClass(sslCLASS_REF());
    if (!cls.pClass)
        return false;

    if (!RegisterSslFuncCb(cls))
    {
        _apLog("~BASS,Error~aiBASS::Init(): failed to register SSL func callbacks");
        return false;
    }

    dsTSTRING<char> sslSrc = rootIf.GetStr("#ssl", dsTSTRING<char>(), 0);
    if (sslSrc.CStr()[0])
    {
        sslERROR err = cls.AddScript(sslSrc.CStr(), "parsing bass.cls ssl");
        if (err.id != SSL_ERR_NO_ERROR)
            return false;
    }

    dsSTRID objName(sslObjName, false);
    gsSslSystem->NameClass(cls, objName);
    cls.SetDbgInfo(sslObjName);
    sslObj = gsSslSystem->AddObject(sslObjName.CStr(), cls, (sslCB_HOST *)this);
    sslObj.AllowDelayedFuncs(1);
    return true;
}
