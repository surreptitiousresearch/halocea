#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiBRAIN_DESC_sslfunc_boundary.h"
#include "../../headers/ws/dsc/dscDESC.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ssl/sslERROR.h"
#include "../../headers/ws/ssl/sslCB_HOST.h"
#include "../../headers/ws/ia/iaIACTOR.h"
#include "../../headers/ws/ds/dsDATA.h"

struct sslSYSTEM;
extern sslSYSTEM *gsSslSystem; // global ssl script system (haloInit_boundary.h)

// aiBRAIN::FindExPartner(float) @ 0x8324CF48
// ?FindExPartner@aiBRAIN@@QAAPAViaIACTOR@@M@Z
//
// Run the descriptor's "FindExPartner" SSL function with search `range` as its single argument; its
// return value is a script object reference to the found exclamation partner. Resolve that reference
// against the global script system and return the referenced object's callback host (null when empty).
iaIACTOR *aiBRAIN::FindExPartner(float range)
{
    dsDATA arg;
    arg.type = nullptr;
    arg.StoreValue<float>(range);

    dsDATA ret;
    ret.type = nullptr;

    // RAII: the returned sslERROR temporary and the empty caller ref self-release on return.
    sslObject.CallFunc(aiBRAIN_DESC_SslFnFindExPartner(spDesc.pointee), 1, &arg, &ret, sslOBJ_REF());

    sslOBJ_REF partnerObj;
    partnerObj.Set(ret, gsSslSystem); // resolve the returned reference against the script system

    // GetCbHost yields the referenced object's callback host; the partner actor IS that host (the
    // DB mangle types the result iaIACTOR*).
    sslCB_HOST *host = partnerObj.GetCbHost();
    return reinterpret_cast<iaIACTOR *>(host);
    // partnerObj, ret and arg destructors self-release at scope end.
}
