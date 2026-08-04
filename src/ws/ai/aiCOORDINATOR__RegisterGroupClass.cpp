// aiCOORDINATOR::RegisterGroupClass @ 0x832372D0
// ?RegisterGroupClass@aiCOORDINATOR@@UAA_NABV?$dsTSTRING@D@@ABVdsSTRID@@@Z
//
// Register a coordinated-group SSL class: load "<sslClassName>.ssl" as a parametric-script file,
// create (optionally deriving from a named parentClass) and name the SSL class, register the group's
// native SSL func callbacks, compile the class's embedded "#ssl" script, then record the class under
// the (name, type) pair. Returns true on success; each failure stage logs a "~AI,Error~" diagnostic
// and returns false. Follows the same PS-load/SSL-class idiom as aiBASS::Init.
//
// DEVIATION: the decompile aliases the PS load buffer, the derived key-value section, and the root
// section onto overlapping stack slots and open-codes the dsTSTRING refcount juggling; the receiver
// objects are reconstructed from the mangled callee signatures and data flow. The class PS is loaded
// through the psSYSTEM_LoadPsFromFile boundary shim (psSYSTEM.h models `psSystem` as psSECTION for
// the txm consumer, so this TU declares the psSYSTEM view locally).
#include "../../headers/ws/ai/aiCOORDINATOR.h"
#include "../../headers/ws/ai/aiCOORD_GROUP_boundary.h"
#include "../../headers/ws/ps/psSECTION.h"
#include "../../headers/ws/ps/psSECTION_INTERFACE.h"
#include "../../headers/ws/ssl/sslSYSTEM.h"
#include "../../headers/ws/ssl/sslCLASS.h"
#include "../../headers/ws/ssl/sslCLASS_REF.h"
#include "../../headers/ws/ssl/sslERROR.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsSTRID.h"
#include "../../headers/ws/ds/dsTSTRING.h"

struct psSYSTEM;                           // ps — parametric-script system (pointer only here)
extern psSYSTEM *psSystem;                 // ?psSystem@@3PAVpsSYSTEM@@A — global ps-script system
// ?LoadPsFromFile@psSYSTEM@@QAA?AVpsSECTION@@V?$dsTSTRING@D@@00H@Z (via boundary shim).
extern psSECTION psSYSTEM_LoadPsFromFile(psSYSTEM *sys, dsTSTRING<char> fileName,
                                         dsTSTRING<char> name, dsTSTRING<char> errHint,
                                         int ignorePreload);
extern sslSYSTEM *gsSslSystem;             // ?gsSslSystem@@3PAVsslSYSTEM@@A — global SSL system
extern int psDataToSection(const dsDATA &kv, psSECTION *out);
extern void _apLog(const char *fmt, ...);  // ?_apLog@@YAXPBDZZ — engine logger  boundary

bool aiCOORDINATOR::RegisterGroupClass(const dsTSTRING<char> &sslClassName, const dsSTRID &groupType)
{
    dsTSTRING<char> fileName = sslClassName + dsTSTRING<char>(".ssl");

    psSECTION root = psSYSTEM_LoadPsFromFile(
        psSystem, fileName, dsTSTRING<char>("root"), dsTSTRING<char>(), 0);

    psSECTION_INTERFACE &rootIf = (psSECTION_INTERFACE &)root;
    if (rootIf.IsNull() || rootIf.IsEmpty())
    {
        _apLog("~AI,Error~RegisterGroupClass(): %s(%s): failed to load class PS",
               sslClassName.CStr(), groupType.CStr());
        return false;
    }

    psSECTION section;
    psDataToSection(rootIf.GetKeyValue(0), &section);

    // Optional parent class: derive from the previously-registered group class of the given name.
    sslCLASS_REF parentCls;
    if (rootIf.IsKeyExist("parentClass", 0))
    {
        dsTSTRING<char> parentName;
        if (rootIf.GetDSString("parentClass", &parentName, 0))
            parentCls = GetGroupSslClass(parentName);
    }

    sslCLASS_REF cls = gsSslSystem->AddClass(parentCls);
    if (!cls.pClass)
    {
        _apLog("~AI,Error~RegisterGroupClass(): %s(%s): failed to create SSL class",
               sslClassName.CStr(), groupType.CStr());
        return false;
    }

    gsSslSystem->NameClass(cls, dsSTRID(sslClassName, false));
    cls.SetDbgInfo(sslClassName);

    // Only a root (parent-less) group class needs its native func callbacks registered; a derived
    // class inherits them from its parent.
    if (!parentCls.pClass && !aiCOORD_GROUP::RegisterSslFuncCb(cls))
    {
        _apLog("~AI,Error~RegisterGroupClass(): %s(%s): failed to register SSL func callbacks",
               sslClassName.CStr(), groupType.CStr());
        return false;
    }

    dsTSTRING<char> sslSrc = rootIf.GetStr("#ssl", dsTSTRING<char>(), 0);
    if (!sslSrc.CStr()[0])
    {
        _apLog("~AI,Error~RegisterGroupClass(): %s(%s): failed to read class script from PS",
               sslClassName.CStr(), groupType.CStr());
        return false;
    }

    sslERROR err = cls.AddScript(sslSrc.CStr(), (dsTSTRING<char>("parsing ") + fileName).CStr());
    if (err.id != SSL_ERR_NO_ERROR)
    {
        _apLog("~AI,Error~RegisterGroupClass(): %s(%s): failed to parse class script",
               sslClassName.CStr(), groupType.CStr());
        return false;
    }

    AddGroupSslClass(cls, sslClassName, groupType);
    _apLog("~AI~RegisterGroupClass(): %s(%s): group class successfully added",
           sslClassName.CStr(), groupType.CStr());
    cls.pClass->LockElems();
    return true;
}
