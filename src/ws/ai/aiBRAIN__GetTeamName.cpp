#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiBRAIN_DESC_sslfunc_boundary.h"
#include "../../headers/ws/dsc/dscDESC.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ssl/sslERROR.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/ds/dsDATA_SIMPLE_WRAPPER.h"

// aiBRAIN::GetTeamName() @ 0x83259130
// ?GetTeamName@aiBRAIN@@QAA?AV?$dsTSTRING@D@@XZ
//
// Run the descriptor's "GetTeamName" SSL function (no args); unbox the returned dsDATA back into a
// dsTSTRING<char> and return it by value.
dsTSTRING<char> aiBRAIN::GetTeamName()
{
    dsDATA ret;
    ret.type = nullptr;

    // RAII: the returned sslERROR temporary and the empty caller ref self-release on return.
    sslObject.CallFunc(aiBRAIN_DESC_SslFnGetTeamName(spDesc.pointee), 0, nullptr, &ret, sslOBJ_REF());

    dsDATA_SIMPLE_WRAPPER<dsTSTRING<char> > result(ret, nullptr);
    return result.val;
    // result + ret destructors self-release at scope end.
}
