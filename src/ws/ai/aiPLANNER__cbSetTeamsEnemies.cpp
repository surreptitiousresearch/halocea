#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/ds/dsDATA_SIMPLE_WRAPPER.h"

// aiPLANNER::cbSetTeamsEnemies(int, dsDATA*, dsDATA&, sslOBJ_REF) @ 0x8325A1A8
// ?cbSetTeamsEnemies@aiPLANNER@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z
//
// SSL "SetTeamsEnemies" callback: unbox argv[0] (teamFrom) and argv[1] (teamTo), forward to
// implSetTeamsEnemies and store the returned bool into `retVal`. `caller` releases on return.
void aiPLANNER::cbSetTeamsEnemies(int /*argc*/, dsDATA *argv, dsDATA &retVal, sslOBJ_REF /*caller*/)
{
    dsDATA_SIMPLE_WRAPPER<dsTSTRING<char> > teamTo(argv[1], nullptr);
    dsDATA_SIMPLE_WRAPPER<dsTSTRING<char> > teamFrom(argv[0], nullptr);

    bool res = implSetTeamsEnemies(teamFrom.val, teamTo.val);

    dsDATA out;
    out.type = nullptr;
    out.StoreValue<bool>(res);
    retVal.StoreValue(out);
    // wrapper + out destructors self-release at scope end.
}
