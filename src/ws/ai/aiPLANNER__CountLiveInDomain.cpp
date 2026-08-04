#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/ai/aiPLANNER_ssl_boundary.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ssl/sslERROR.h"
#include "../../headers/ws/ia/iaIACTOR.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/ds/dsCONVERTION_TYPE.h"

// aiPLANNER::CountLiveInDomain(iaIACTOR*, const dsTSTRING<char>&) @ 0x83254C48
// ?CountLiveInDomain@aiPLANNER@@QAAHPAViaIACTOR@@ABV?$dsTSTRING@D@@@Z
//
// Run the descriptor's "CountLiveInDomain" SSL function with (`actor`'s SSL object reference,
// `domain`) marshalled into a contiguous dsDATA[2]; returns the int count.
int aiPLANNER::CountLiveInDomain(iaIACTOR *actor, const dsTSTRING<char> &domain)
{
    sslOBJ_REF ref; // empty ref when actor is null
    if (actor)
        ref = actor->sslObject;
    dsDATA asData = ref; // sslOBJ_REF::operator dsDATA()

    dsDATA args[2];
    args[0].type = nullptr;
    args[0].StoreValue(asData);
    args[1].type = nullptr;
    args[1].StoreValue<dsTSTRING<char> >(domain);

    dsDATA retVal;
    retVal.type = nullptr;

    // RAII: the returned sslERROR temporary and the empty caller ref self-release on return.
    aiPLANNER_IA(this).sslObject.CallFunc(
        aiPLANNER_Desc(this)->sslFuncId_CountLiveInDomain, 2, args, &retVal, sslOBJ_REF());

    int result = 0;
    retVal.GetValue<int>(result, DSD_CONV_RETRIEVE);
    return result;
    // args[] and retVal destructors release their stored values at scope end.
}
