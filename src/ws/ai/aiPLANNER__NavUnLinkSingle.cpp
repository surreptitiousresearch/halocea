#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/ai/aiPLANNER_DESC_sslfunc_boundary.h"
#include "../../headers/ws/dsc/dscDESC.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ssl/sslERROR.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/ds/dsCONVERTION_TYPE.h"

// aiPLANNER::NavUnLinkSingle(const dsTSTRING<char>&, const dsTSTRING<char>&, const dsTSTRING<char>&) @ 0x832524E0
// SSL op: remove the single scripted nav link in nav-system sysName.
// Marshals the arguments into a contiguous dsDATA[3] and runs the descriptor's "NavUnLinkSingle" SSL
// function via sslObject.CallFunc, on behalf of an empty (script-root) caller.
void aiPLANNER::NavUnLinkSingle(const dsTSTRING<char> &sysName, const dsTSTRING<char> &wpA, const dsTSTRING<char> &wpB)
{
    dsDATA args[3];
    args[0].type = nullptr;
    args[0].StoreValue<dsTSTRING<char> >(sysName);
    args[1].type = nullptr;
    args[1].StoreValue<dsTSTRING<char> >(wpA);
    args[2].type = nullptr;
    args[2].StoreValue<dsTSTRING<char> >(wpB);

    // Void SSL op: no return value marshalled (retVal = null). RAII: the returned sslERROR
    // temporary and the empty caller ref self-release on return.
    aiPLANNER_SslObject(this)->CallFunc(aiPLANNER_DESC_SslFnNavUnLinkSingle(aiPLANNER_SpDescPointee(this)), 3, args, nullptr, sslOBJ_REF());
    // args[] destructors release their stored values at scope end.
}
