#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiBRAIN_DESC_sslfunc_boundary.h"
#include "../../headers/ws/dsc/dscDESC.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ssl/sslERROR.h"
#include "../../headers/ws/ds/dsDATA.h"

struct sslSYSTEM;
extern sslSYSTEM *gsSslSystem; // global ssl script system (haloInit_boundary.h)

// aiBRAIN::GetGroup() @ 0x83250320
// ?GetGroup@aiBRAIN@@QAA?AVsslOBJ_REF@@XZ
//
// Run the descriptor's "GetGroup" SSL function (no args); its return value is a script object
// reference to the brain's coordinated group. Resolve that reference against the global script system
// and return it (by value; an empty ref when the brain is in no group).
sslOBJ_REF aiBRAIN::GetGroup()
{
    dsDATA ret;
    ret.type = nullptr;

    // RAII: the returned sslERROR temporary and the empty caller ref self-release on return.
    sslObject.CallFunc(aiBRAIN_DESC_SslFnGetGroup(spDesc.pointee), 0, nullptr, &ret, sslOBJ_REF());

    sslOBJ_REF group;
    group.Set(ret, gsSslSystem); // resolve the returned reference against the script system
    return group;
    // ret destructor releases its stored value at scope end.
}
