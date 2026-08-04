#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/ds/dsDATA_SIMPLE_WRAPPER.h"

// aiPLANNER::cbNavForceAutolinkWP(int, dsDATA*, dsDATA&, sslOBJ_REF) @ 0x83259D60
// ?cbNavForceAutolinkWP@aiPLANNER@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z
//
// SSL "NavForceAutolinkWP" callback: unbox the waypoint-name string argv[0], forward to
// implNavForceAutolinkWP, box the returned bool into retVal. `caller` self-releases on return.
void aiPLANNER::cbNavForceAutolinkWP(int /*argc*/, dsDATA *argv, dsDATA &retVal, sslOBJ_REF /*caller*/)
{
    dsDATA_SIMPLE_WRAPPER<dsTSTRING<char> > wpName(argv[0], nullptr);
    bool autolinked = implNavForceAutolinkWP(wpName.val);

    dsDATA out;
    out.type = nullptr;
    out.StoreValue<bool>(autolinked);
    retVal.StoreValue(out);
    // out + wrapper destructors self-release at scope end.
}
