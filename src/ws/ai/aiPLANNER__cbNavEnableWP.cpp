#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/ds/dsDATA_SIMPLE_WRAPPER.h"

// aiPLANNER::cbNavEnableWP(int, dsDATA*, dsDATA&, sslOBJ_REF) @ 0x83259CA8
// ?cbNavEnableWP@aiPLANNER@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z
//
// SSL "NavEnableWP" callback: unbox the waypoint-name string argv[0], forward to implNavEnableWP,
// box the returned bool into retVal. `caller` self-releases on return.
void aiPLANNER::cbNavEnableWP(int /*argc*/, dsDATA *argv, dsDATA &retVal, sslOBJ_REF /*caller*/)
{
    dsDATA_SIMPLE_WRAPPER<dsTSTRING<char> > wpName(argv[0], nullptr);
    bool enabled = implNavEnableWP(wpName.val);

    dsDATA out;
    out.type = nullptr;
    out.StoreValue<bool>(enabled);
    retVal.StoreValue(out);
    // out + wrapper destructors self-release at scope end.
}
