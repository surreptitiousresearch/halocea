#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/ai/aiPLANNER_ssl_boundary.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ssl/sslERROR.h"
#include "../../headers/ws/ia/iaIACTOR.h"
#include "../../headers/ws/ds/dsDATA.h"

// aiPLANNER::NoticeGrenadeExplosion(iaIACTOR*) @ 0x83254348
// ?NoticeGrenadeExplosion@aiPLANNER@@QAAXPAViaIACTOR@@@Z
//
// Run the descriptor's "NoticeGrenadeExplosion" SSL function with `actor`'s SSL object reference (an
// empty ref when null) as its single argument (no return value used).
void aiPLANNER::NoticeGrenadeExplosion(iaIACTOR *actor)
{
    sslOBJ_REF ref; // empty ref when actor is null
    if (actor)
        ref = actor->sslObject;
    dsDATA asData = ref; // sslOBJ_REF::operator dsDATA()
    dsDATA arg;
    arg.type = nullptr;
    arg.StoreValue(asData);

    // RAII: the returned sslERROR temporary and the empty caller ref self-release on return.
    aiPLANNER_IA(this).sslObject.CallFunc(
        aiPLANNER_Desc(this)->sslFuncId_NoticeGrenadeExplosion, 1, &arg, nullptr, sslOBJ_REF());
    // arg destructor releases its stored value at scope end.
}
