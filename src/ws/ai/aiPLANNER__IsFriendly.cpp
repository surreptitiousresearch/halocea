#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/ai/aiPLANNER_ssl_boundary.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ssl/sslERROR.h"
#include "../../headers/ws/ia/iaIACTOR.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsCONVERTION_TYPE.h"

// aiPLANNER::IsFriendly(iaIACTOR*, iaIACTOR*) @ 0x83253B98
// ?IsFriendly@aiPLANNER@@QAA_NPAViaIACTOR@@0@Z
//
// Run the descriptor's "IsFriendly" SSL function with the two actors' SSL object references (empty
// refs when null) marshalled into a contiguous dsDATA[2]; returns its bool result.
bool aiPLANNER::IsFriendly(iaIACTOR *obj1, iaIACTOR *obj2)
{
    sslOBJ_REF ref1; // empty ref when obj1 is null
    if (obj1)
        ref1 = obj1->sslObject;
    dsDATA asData1 = ref1; // sslOBJ_REF::operator dsDATA()
    dsDATA args[2];
    args[0].type = nullptr;
    args[0].StoreValue(asData1);

    sslOBJ_REF ref2; // empty ref when obj2 is null
    if (obj2)
        ref2 = obj2->sslObject;
    dsDATA asData2 = ref2;
    args[1].type = nullptr;
    args[1].StoreValue(asData2);

    dsDATA retVal;
    retVal.type = nullptr;

    // RAII: the returned sslERROR temporary and the empty caller ref self-release on return.
    aiPLANNER_IA(this).sslObject.CallFunc(
        aiPLANNER_Desc(this)->sslFuncId_IsFriendly, 2, args, &retVal, sslOBJ_REF());

    bool result = false;
    retVal.GetValue<bool>(result, DSD_CONV_RETRIEVE);
    return result;
    // args[] and retVal destructors release their stored values at scope end.
}
