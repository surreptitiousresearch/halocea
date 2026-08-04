#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/ai/aiPLANNER_DESC_sslfunc_boundary.h"
#include "../../headers/ws/dsc/dscDESC.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ssl/sslERROR.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/ds/dsCONVERTION_TYPE.h"

// aiPLANNER::SetTeamsEnemies(const dsTSTRING<char>&, const dsTSTRING<char>&) @ 0x83253778
// SSL op: mark teams teamA and teamB as mutual enemies.
// Marshals the arguments into a contiguous dsDATA[2] and runs the descriptor's "SetTeamsEnemies" SSL
// function via sslObject.CallFunc, on behalf of an empty (script-root) caller.
bool aiPLANNER::SetTeamsEnemies(const dsTSTRING<char> &teamA, const dsTSTRING<char> &teamB)
{
    dsDATA args[2];
    args[0].type = nullptr;
    args[0].StoreValue<dsTSTRING<char> >(teamA);
    args[1].type = nullptr;
    args[1].StoreValue<dsTSTRING<char> >(teamB);

    dsDATA retVal;
    retVal.type = nullptr;

    // RAII: the returned sslERROR temporary and the empty caller ref self-release on return.
    aiPLANNER_SslObject(this)->CallFunc(aiPLANNER_DESC_SslFnSetTeamsEnemies(aiPLANNER_SpDescPointee(this)), 2, args, &retVal, sslOBJ_REF());

    bool result = false;
    retVal.GetValue<bool>(result, DSD_CONV_RETRIEVE);
    return result;
    // args[] and retVal destructors release their stored values at scope end.
}
