#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ssl/sslCB_HOST.h"
#include "../../headers/ws/ssl/sslERROR.h" // sslOBJ_REF::Set returns sslERROR by value
#include "../../headers/ws/ia/iaIACTOR.h"
#include "../../headers/ws/ds/dsDATA.h"

struct sslSYSTEM;
extern sslSYSTEM *gsSslSystem; // global ssl script system (haloInit_boundary.h)

// aiPLANNER::cbIsFriendly @ 0x83253DA0
// ?cbIsFriendly@aiPLANNER@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z
//
// SSL callback for IsFriendly: resolve the two object references in argv[0]/argv[1] against the
// script system to actors, forward them (argv[0], argv[1] order) to implIsFriendly, and store the
// resulting bool into `retVal`. `caller` releases on return.
void aiPLANNER::cbIsFriendly(int /*argc*/, dsDATA *argv, dsDATA &retVal, sslOBJ_REF /*caller*/)
{
    sslOBJ_REF ref1;
    ref1.Set(argv[1], gsSslSystem); // resolve argv[1]'s reference against the script system
    iaIACTOR *obj2 = reinterpret_cast<iaIACTOR *>(ref1.GetCbHost());

    sslOBJ_REF ref0;
    ref0.Set(argv[0], gsSslSystem); // resolve argv[0]'s reference against the script system
    iaIACTOR *obj1 = reinterpret_cast<iaIACTOR *>(ref0.GetCbHost());

    bool result = implIsFriendly(obj1, obj2);
    retVal.StoreValue<bool>(result);
    // ref0/ref1 destructors self-release at scope end.
}
