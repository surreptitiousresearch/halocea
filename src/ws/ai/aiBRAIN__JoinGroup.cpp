#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiBRAIN_DESC_sslfunc_boundary.h"
#include "../../headers/ws/dsc/dscDESC.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ssl/sslERROR.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/ds/dsCONVERTION_TYPE.h"

// aiBRAIN::JoinGroup(const dsTSTRING<char>&, const dsTSTRING<char>&) @ 0x8324FF40
// ?JoinGroup@aiBRAIN@@QAA_NABV?$dsTSTRING@D@@0@Z
//
// Run the descriptor's "JoinGroup" SSL function with (group, tag) and return its bool result. The two
// string arguments are marshalled into a contiguous dsDATA[2].
bool aiBRAIN::JoinGroup(const dsTSTRING<char> &group, const dsTSTRING<char> &tag)
{
    dsDATA args[2];
    args[0].type = nullptr;
    args[0].StoreValue<dsTSTRING<char> >(group);
    args[1].type = nullptr;
    args[1].StoreValue<dsTSTRING<char> >(tag);

    dsDATA retVal;
    retVal.type = nullptr;

    // RAII: the returned sslERROR temporary and the empty caller ref self-release on return.
    sslObject.CallFunc(aiBRAIN_DESC_SslFnJoinGroup(spDesc.pointee), 2, args, &retVal, sslOBJ_REF());

    bool result = false;
    retVal.GetValue<bool>(result, DSD_CONV_RETRIEVE);
    return result;
    // args[] and retVal destructors release their stored values at scope end.
}
