#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ssl/sslCB_HOST.h"
#include "../../headers/ws/ssl/sslERROR.h" // sslOBJ_REF::Set returns sslERROR by value
#include "../../headers/ws/ia/iaIACTOR.h"
#include "../../headers/ws/ds/dsDATA.h"

struct sslSYSTEM;
extern sslSYSTEM *gsSslSystem; // global ssl script system (haloInit_boundary.h)

// aiPLANNER::cbAddEnemy @ 0x83255238
// ?cbAddEnemy@aiPLANNER@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z
//
// SSL callback: resolve the object reference in argv[0] to an actor, forward it to implAddEnemy and
// store the returned bool into `retVal`. `caller` releases on return.
void aiPLANNER::cbAddEnemy(int /*argc*/, dsDATA *argv, dsDATA &retVal, sslOBJ_REF /*caller*/)
{
    sslOBJ_REF ref;
    ref.Set(argv[0], gsSslSystem); // resolve argv[0]'s reference against the script system
    iaIACTOR *actor = reinterpret_cast<iaIACTOR *>(ref.GetCbHost());

    bool res = implAddEnemy(actor);

    dsDATA out;
    out.type = nullptr;
    out.StoreValue<bool>(res);
    retVal.StoreValue(out);
    // ref and out destructors self-release at scope end.
}
