#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ssl/sslCB_HOST.h"
#include "../../headers/ws/ssl/sslERROR.h" // sslOBJ_REF::Set returns sslERROR by value
#include "../../headers/ws/ia/iaIACTOR.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/ds/dsDATA_SIMPLE_WRAPPER.h"

struct sslSYSTEM;
extern sslSYSTEM *gsSslSystem; // global ssl script system (haloInit_boundary.h)

// aiPLANNER::cbCountLiveInDomain(int, dsDATA*, dsDATA&, sslOBJ_REF) @ 0x8325A4A8
// ?cbCountLiveInDomain@aiPLANNER@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z
//
// SSL "CountLiveInDomain" callback: unbox argv[1] (team name), resolve argv[0]'s object reference to a
// domain actor, forward both to implCountLiveInDomain and store the int result into `retVal`.
// `caller` releases on return.
void aiPLANNER::cbCountLiveInDomain(int /*argc*/, dsDATA *argv, dsDATA &retVal, sslOBJ_REF /*caller*/)
{
    dsDATA_SIMPLE_WRAPPER<dsTSTRING<char> > team(argv[1], nullptr);

    sslOBJ_REF ref;
    ref.Set(argv[0], gsSslSystem); // resolve argv[0]'s reference against the script system
    iaIACTOR *dom = reinterpret_cast<iaIACTOR *>(ref.GetCbHost());

    int res = implCountLiveInDomain(dom, team.val);

    dsDATA out;
    out.type = nullptr;
    out.StoreValue<int>(res);
    retVal.StoreValue(out);
    // ref, team, and out destructors self-release at scope end.
}
