#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/ds/dsDATA_SIMPLE_WRAPPER.h"

// aiPLANNER::cbSetTeamsNeutrals(int, dsDATA*, dsDATA&, sslOBJ_REF) @ 0x8325A2A0
// ?cbSetTeamsNeutrals@aiPLANNER@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z
//
// SSL "SetTeamsNeutrals" callback: unbox argv[0] (teamFrom) and argv[1] (teamTo), forward to
// implSetTeamsNeutrals and store the returned bool into `retVal`. `caller` releases on return.
void aiPLANNER::cbSetTeamsNeutrals(int /*argc*/, dsDATA *argv, dsDATA &retVal, sslOBJ_REF /*caller*/)
{
    dsDATA_SIMPLE_WRAPPER<dsTSTRING<char> > teamTo(argv[1], nullptr);
    dsDATA_SIMPLE_WRAPPER<dsTSTRING<char> > teamFrom(argv[0], nullptr);

    bool res = implSetTeamsNeutrals(teamFrom.val, teamTo.val);

    dsDATA out;
    out.type = nullptr;
    out.StoreValue<bool>(res);
    retVal.StoreValue(out);
    // wrapper + out destructors self-release at scope end.
}
