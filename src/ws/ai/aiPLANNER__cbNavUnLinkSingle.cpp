#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/ds/dsDATA_SIMPLE_WRAPPER.h"

// aiPLANNER::cbNavUnLinkSingle(int, dsDATA*, dsDATA&, sslOBJ_REF) @ 0x83259910
// ?cbNavUnLinkSingle@aiPLANNER@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z
//
// SSL "NavUnLinkSingle" callback: unbox three strings (nav-system argv[0], waypoint A argv[1],
// waypoint B argv[2]) and forward them to implNavUnLinkSingle. `caller` self-releases on return.
void aiPLANNER::cbNavUnLinkSingle(int /*argc*/, dsDATA *argv, dsDATA & /*retVal*/, sslOBJ_REF /*caller*/)
{
    dsDATA_SIMPLE_WRAPPER<dsTSTRING<char> > sysName(argv[0], nullptr);
    dsDATA_SIMPLE_WRAPPER<dsTSTRING<char> > wpA(argv[1], nullptr);
    dsDATA_SIMPLE_WRAPPER<dsTSTRING<char> > wpB(argv[2], nullptr);
    implNavUnLinkSingle(sysName.val, wpA.val, wpB.val);
    // wrapper destructors self-release at scope end.
}
