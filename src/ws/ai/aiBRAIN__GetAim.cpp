#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiBRAIN_DESC_sslfunc_boundary.h"
#include "../../headers/ws/dsc/dscDESC.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ssl/sslERROR.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/ds/dsDATA_SIMPLE_WRAPPER.h"

// aiBRAIN::GetAim() @ 0x83256848
// ?GetAim@aiBRAIN@@QAA?AV?$dsTSTRING@D@@XZ
//
// Run the descriptor's "GetAim" SSL function (no args); its return value is the brain's current aim
// name. Unbox the returned dsDATA back into a dsTSTRING<char> and return it by value.
dsTSTRING<char> aiBRAIN::GetAim()
{
    dsDATA ret;
    ret.type = nullptr;

    // RAII: the returned sslERROR temporary and the empty caller ref self-release on return.
    sslObject.CallFunc(aiBRAIN_DESC_SslFnGetAim(spDesc.pointee), 0, nullptr, &ret, sslOBJ_REF());

    // unbox the returned value into a dsTSTRING<char>; the wrapper owns the buffer, so copy it out.
    dsDATA_SIMPLE_WRAPPER<dsTSTRING<char> > aim(ret, nullptr);
    return aim.val;
    // aim + ret destructors self-release at scope end.
}
