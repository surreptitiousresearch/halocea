#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/ai/aiPLANNER_ssl_boundary.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ssl/sslERROR.h"
#include "../../headers/ws/ia/iaIACTOR.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsCONVERTION_TYPE.h"

// aiPLANNER::AddEnemy(iaIACTOR*) @ 0x832550D8
// ?AddEnemy@aiPLANNER@@QAA_NPAViaIACTOR@@@Z
//
// Run the descriptor's "AddEnemy" SSL function with `actor`'s SSL object reference (an empty ref
// when null) as its single argument; returns the script's bool result.
bool aiPLANNER::AddEnemy(iaIACTOR *actor)
{
    sslOBJ_REF ref; // empty ref when actor is null
    if (actor)
        ref = actor->sslObject;
    dsDATA asData = ref; // sslOBJ_REF::operator dsDATA()
    dsDATA arg;
    arg.type = nullptr;
    arg.StoreValue(asData);

    dsDATA retVal;
    retVal.type = nullptr;

    // RAII: the returned sslERROR temporary and the empty caller ref self-release on return.
    aiPLANNER_IA(this).sslObject.CallFunc(
        aiPLANNER_Desc(this)->sslFuncId_AddEnemy, 1, &arg, &retVal, sslOBJ_REF());

    bool result = false;
    retVal.GetValue<bool>(result, DSD_CONV_RETRIEVE);
    return result;
    // arg and retVal destructors release their stored values at scope end.
}
