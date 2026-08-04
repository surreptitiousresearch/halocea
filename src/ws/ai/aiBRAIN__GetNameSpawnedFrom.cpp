#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiBRAIN_DESC_sslfunc_boundary.h"
#include "../../headers/ws/dsc/dscDESC.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ssl/sslERROR.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/ds/dsDATA_SIMPLE_WRAPPER.h"

// aiBRAIN::GetNameSpawnedFrom() @ 0x83257500
// ?GetNameSpawnedFrom@aiBRAIN@@QAA?AV?$dsTSTRING@D@@XZ
//
// Run the descriptor's "GetNameSpawnedFrom" SSL function (no args); its return value is the name
// this brain was spawned from. Unbox the returned dsDATA into a dsTSTRING<char> and return by value.
dsTSTRING<char> aiBRAIN::GetNameSpawnedFrom()
{
    dsDATA ret;
    ret.type = nullptr;

    // RAII: the returned sslERROR temporary and the empty caller ref self-release on return.
    sslObject.CallFunc(aiBRAIN_DESC_SslFnGetNameSpawnedFrom(spDesc.pointee), 0, nullptr, &ret, sslOBJ_REF());

    dsDATA_SIMPLE_WRAPPER<dsTSTRING<char> > name(ret, nullptr);
    return name.val;
    // name + ret destructors self-release at scope end.
}
