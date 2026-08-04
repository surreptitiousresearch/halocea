#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/ai/aiPLANNER_ssl_boundary.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ssl/sslERROR.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsTSTRING.h"

struct sslSYSTEM;
extern sslSYSTEM *gsSslSystem; // global ssl script system (haloInit_boundary.h)

// aiPLANNER::FindGroupByName(const dsTSTRING<char>&) @ 0x83254A88
// ?FindGroupByName@aiPLANNER@@QAA?AVsslOBJ_REF@@ABV?$dsTSTRING@D@@@Z
//
// Run the descriptor's "FindGroupByName" SSL function with `grpName`; its return value is a script
// object reference to the named group. Resolve that reference against the global script system and
// return it (empty when no such group exists).
//
// Distinct from implFindGroupByName (0x8322ECB8), which looks the group up directly through the
// embedded coordinator; this is the SSL round-trip emitter side.
sslOBJ_REF aiPLANNER::FindGroupByName(const dsTSTRING<char> &grpName)
{
    dsDATA arg;
    arg.type = nullptr;
    arg.StoreValue<dsTSTRING<char> >(grpName);

    dsDATA retVal;
    retVal.type = nullptr;

    // RAII: the returned sslERROR temporary and the empty caller ref self-release on return.
    aiPLANNER_IA(this).sslObject.CallFunc(
        aiPLANNER_Desc(this)->sslFuncId_FindGroupByName, 1, &arg, &retVal, sslOBJ_REF());

    sslOBJ_REF result;
    result.Set(retVal, gsSslSystem); // resolve the returned reference against the script system
    return result;
    // arg and retVal destructors release their stored values at scope end.
}
