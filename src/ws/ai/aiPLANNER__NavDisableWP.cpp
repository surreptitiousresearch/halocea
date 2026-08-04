#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/ai/aiPLANNER_DESC_sslfunc_boundary.h"
#include "../../headers/ws/dsc/dscDESC.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ssl/sslERROR.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/ds/dsCONVERTION_TYPE.h"

// aiPLANNER::NavDisableWP(const dsTSTRING<char>&) @ 0x83252B70
// SSL op: disable waypoint wpName.
// Marshals the arguments into a contiguous dsDATA[1] and runs the descriptor's "NavDisableWP" SSL
// function via sslObject.CallFunc, on behalf of an empty (script-root) caller.
bool aiPLANNER::NavDisableWP(const dsTSTRING<char> &wpName)
{
    dsDATA args[1];
    args[0].type = nullptr;
    args[0].StoreValue<dsTSTRING<char> >(wpName);

    dsDATA retVal;
    retVal.type = nullptr;

    // RAII: the returned sslERROR temporary and the empty caller ref self-release on return.
    aiPLANNER_SslObject(this)->CallFunc(aiPLANNER_DESC_SslFnNavDisableWP(aiPLANNER_SpDescPointee(this)), 1, args, &retVal, sslOBJ_REF());

    bool result = false;
    retVal.GetValue<bool>(result, DSD_CONV_RETRIEVE);
    return result;
    // args[] and retVal destructors release their stored values at scope end.
}
