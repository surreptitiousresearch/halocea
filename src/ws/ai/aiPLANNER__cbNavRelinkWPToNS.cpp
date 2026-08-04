#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/ds/dsDATA_SIMPLE_WRAPPER.h"

// aiPLANNER::cbNavRelinkWPToNS(int, dsDATA*, dsDATA&, sslOBJ_REF) @ 0x83259E18
// ?cbNavRelinkWPToNS@aiPLANNER@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z
//
// SSL "NavRelinkWPToNS" callback: unbox two strings (waypoint argv[0], nav-system argv[1]), forward
// to implNavRelinkWPToNS, box the returned bool into retVal. `caller` self-releases on return.
void aiPLANNER::cbNavRelinkWPToNS(int /*argc*/, dsDATA *argv, dsDATA &retVal, sslOBJ_REF /*caller*/)
{
    dsDATA_SIMPLE_WRAPPER<dsTSTRING<char> > wpName(argv[0], nullptr);
    dsDATA_SIMPLE_WRAPPER<dsTSTRING<char> > nsName(argv[1], nullptr);
    bool relinked = implNavRelinkWPToNS(wpName.val, nsName.val);

    dsDATA out;
    out.type = nullptr;
    out.StoreValue<bool>(relinked);
    retVal.StoreValue(out);
    // out + wrapper destructors self-release at scope end.
}
