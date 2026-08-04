#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ssl/sslCB_HOST.h"
#include "../../headers/ws/ssl/sslERROR.h" // sslOBJ_REF::Set returns sslERROR by value
#include "../../headers/ws/ia/iaIACTOR.h"
#include "../../headers/ws/ds/dsDATA.h"

struct sslSYSTEM;
extern sslSYSTEM *gsSslSystem; // global ssl script system (haloInit_boundary.h)

// aiBRAIN::cbGetDistToObj @ 0x8324DFC8
// ?cbGetDistToObj@aiBRAIN@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z
//
// SSL callback: resolve the object reference in `argv` against the script system to an actor, forward
// it to implGetDistToObj, and store the resulting float distance into `retVal`. `caller` releases on
// return.
void aiBRAIN::cbGetDistToObj(int /*argc*/, dsDATA *argv, dsDATA &retVal, sslOBJ_REF /*caller*/)
{
    sslOBJ_REF objRef;
    objRef.Set(*argv, gsSslSystem); // resolve argv's reference against the script system

    // GetCbHost yields the referenced object's callback host; the actor IS that host.
    iaIACTOR *obj = reinterpret_cast<iaIACTOR *>(objRef.GetCbHost());

    float dist = implGetDistToObj(obj);
    retVal.StoreValue<float>(dist);
    // objRef destructor self-releases at scope end.
}
