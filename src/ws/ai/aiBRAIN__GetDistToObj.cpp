#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiBRAIN_DESC_sslfunc_boundary.h"
#include "../../headers/ws/dsc/dscDESC.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ssl/sslERROR.h"
#include "../../headers/ws/ia/iaIACTOR.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsCONVERTION_TYPE.h"

// aiBRAIN::GetDistToObj(iaIACTOR*) @ 0x8324DE60
// ?GetDistToObj@aiBRAIN@@QAAMPAViaIACTOR@@@Z
//
// Run the descriptor's "GetDistToObj" SSL function with `obj`'s SSL object reference (an empty ref
// when null) as its single argument, and return the float distance result.
float aiBRAIN::GetDistToObj(iaIACTOR *obj)
{
    sslOBJ_REF ref; // empty ref when obj is null
    if (obj)
        ref = obj->sslObject;

    dsDATA asData = ref; // sslOBJ_REF::operator dsDATA()
    dsDATA arg;
    arg.type = nullptr;
    arg.StoreValue(asData);

    dsDATA retVal;
    retVal.type = nullptr;

    // RAII: the returned sslERROR temporary and the empty caller ref self-release on return.
    sslObject.CallFunc(aiBRAIN_DESC_SslFnGetDistToObj(spDesc.pointee), 1, &arg, &retVal, sslOBJ_REF());

    float result = 0.0f;
    retVal.GetValue<float>(result, DSD_CONV_RETRIEVE);
    return result;
    // arg and retVal destructors release their stored values at scope end.
}
