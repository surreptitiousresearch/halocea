#include "../../headers/ws/ai/aiSQUAD_DESC.h"
#include "../../headers/ws/ai/aiSQUAD.h"        // aiSQUAD::cb* SSL callbacks
#include "../../headers/ws/ent/entDESC.h"       // entDESC::InitSslClass (base)
#include "../../headers/ws/ssl/sslCLASS_REF.h"  // AddCbFunc / sslCB_MEMBER_FN
#include "../../headers/ws/ssl/sslERROR.h"      // sslERROR
#include "../../headers/ws/ds/dsTSTRING.h"
#include <string.h>                             // memcpy

// aiSQUAD_DESC::InitSslClass(const dsTSTRING<char>&) @ 0x832A9A78
// ?InitSslClass@aiSQUAD_DESC@@UAAHABV?$dsTSTRING@D@@@Z
//
// One-time registration of the squad SSL class. Chains to the entDESC base init and (guarded by
// isSslClassInited) unshares the shared sslClass, then registers the squad's seven native callback
// functions (AddCbFunc), storing each id into the sslFuncId_ tail. The first registration error
// aborts (returning 0); the fall-through returns 1.
//
// DEVIATIONS mirror aiBRAIN_DESC::InitSslClass:
//  - The entDESC base's sslClass sub-object lives at base offset 0x10 (sslDESC::sslClass); taken by
//    typed reference here rather than fabricating the base layout.
//  - Each callback is a non-virtual aiSQUAD member fn registered through AddCbFunc with the host
//    bound as the leading argument; represented via a bitwise member-to-member reinterpret (the
//    corpus's AsCbMemberFn idiom), which is equivalent to the binary's plain-code-address register.
//  - The errHint passed to every call is the fixed empty-string byte (byte_8200155A), not `hintErr`.

// Reinterpret an aiSQUAD member-callback pointer as the sslCB_HOST member-callback type AddCbFunc
// expects; a direct pmf-to-pmf reinterpret_cast is ill-formed under MSVC, so copy bitwise.
static sslCB_MEMBER_FN AsCbMemberFn(void (aiSQUAD::*fn)(int, dsDATA *, dsDATA &, sslOBJ_REF))
{
    sslCB_MEMBER_FN out = nullptr;
    memcpy(&out, &fn, sizeof(out));
    return out;
}

int aiSQUAD_DESC::InitSslClass(const dsTSTRING<char> &hintErr)
{
    static const char *const kEmptyErrHint = ""; // byte_8200155A

    if (!reinterpret_cast<entDESC *>(this)->entDESC::InitSslClass(hintErr))
        return 0;

    if (this->isSslClassInited.val)
        return 1;

    this->isSslClassInited.val = 1;
    this->UnshareSslClass(false, false);

    // sslDESC::sslClass @ base+0x10 (opaque entDESC base span).
    sslCLASS_REF &sslClass =
        *reinterpret_cast<sslCLASS_REF *>(reinterpret_cast<char *>(this) + 0x10);

    sslERROR err;

    err = sslClass.AddCbFunc("OnCreate()",
                             AsCbMemberFn(&aiSQUAD::cbOnCreate),
                             kEmptyErrHint, &this->sslFuncId_OnCreate);
    if (err.id)
        return 0;

    err = sslClass.AddCbFunc("OnDbgPause()",
                             AsCbMemberFn(&aiSQUAD::cbOnDbgPause),
                             kEmptyErrHint, &this->sslFuncId_OnDbgPause);
    if (err.id)
        return 0;

    err = sslClass.AddCbFunc("OnDbgResume()",
                             AsCbMemberFn(&aiSQUAD::cbOnDbgResume),
                             kEmptyErrHint, &this->sslFuncId_OnDbgResume);
    if (err.id)
        return 0;

    err = sslClass.AddCbFunc("GetGroup() : object",
                             AsCbMemberFn(&aiSQUAD::cbGetGroup),
                             kEmptyErrHint, &this->sslFuncId_GetGroup);
    if (err.id)
        return 0;

    err = sslClass.AddCbFunc("GetDistMemberToFront(memberIdx : int = 0, frontName : string =\"NONE\") : float",
                             AsCbMemberFn(&aiSQUAD::cbGetDistMemberToFront),
                             kEmptyErrHint, &this->sslFuncId_GetDistMemberToFront);
    if (err.id)
        return 0;

    err = sslClass.AddCbFunc("GetDistMemberToFrontTarget(memberIdx : int = 0, frontName : string =\"NONE\") : float",
                             AsCbMemberFn(&aiSQUAD::cbGetDistMemberToFrontTarget),
                             kEmptyErrHint, &this->sslFuncId_GetDistMemberToFrontTarget);
    if (err.id)
        return 0;

    err = sslClass.AddCbFunc("IsMemberInFront(memberIdx : int = 0, frontName : string =\"NONE\") : bool",
                             AsCbMemberFn(&aiSQUAD::cbIsMemberInFront),
                             kEmptyErrHint, &this->sslFuncId_IsMemberInFront);
    if (err.id)
        return 0;

    return 1;
}
