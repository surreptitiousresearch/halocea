#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiBRAIN_DESC_sslfunc_boundary.h"
#include "../../headers/ws/dsc/dscDESC.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ssl/sslERROR.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/ds/dsDATA_SIMPLE_WRAPPER.h"

// aiBRAIN::AIGetCurWeaponClassName() @ 0x83256DF0
// ?AIGetCurWeaponClassName@aiBRAIN@@QAA?AV?$dsTSTRING@D@@XZ
//
// Run the descriptor's "AIGetCurWeaponClassName" SSL function (no args); the result string is unboxed
// out of the returned dsDATA and returned by value (ref-counted copy of the buffer).
dsTSTRING<char> aiBRAIN::AIGetCurWeaponClassName()
{
    dsDATA ret;
    ret.type = nullptr;

    // RAII: the returned sslERROR temporary and the empty caller ref self-release on return.
    sslObject.CallFunc(aiBRAIN_DESC_SslFnAIGetCurWeaponClassName(spDesc.pointee), 0, nullptr, &ret, sslOBJ_REF());

    dsDATA_SIMPLE_WRAPPER<dsTSTRING<char> > result(ret, nullptr);
    return result.val;
    // ret destructor releases its stored value at scope end.
}
