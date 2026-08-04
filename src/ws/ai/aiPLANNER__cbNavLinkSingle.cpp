#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/ds/dsDATA_SIMPLE_WRAPPER.h"

// aiPLANNER::cbNavLinkSingle(int, dsDATA*, dsDATA&, sslOBJ_REF) @ 0x832596F8
// ?cbNavLinkSingle@aiPLANNER@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z
//
// SSL "NavLinkSingle" callback: unbox four strings (nav-system argv[0], waypoint A argv[1], waypoint
// B argv[2], smart-object argv[3]) and forward them to implNavLinkSingle. `caller` releases on return.
void aiPLANNER::cbNavLinkSingle(int /*argc*/, dsDATA *argv, dsDATA & /*retVal*/, sslOBJ_REF /*caller*/)
{
    dsDATA_SIMPLE_WRAPPER<dsTSTRING<char> > sysName(argv[0], nullptr);
    dsDATA_SIMPLE_WRAPPER<dsTSTRING<char> > wpA(argv[1], nullptr);
    dsDATA_SIMPLE_WRAPPER<dsTSTRING<char> > wpB(argv[2], nullptr);
    dsDATA_SIMPLE_WRAPPER<dsTSTRING<char> > smtObjName(argv[3], nullptr);
    implNavLinkSingle(sysName.val, wpA.val, wpB.val, smtObjName.val);
    // wrapper destructors self-release at scope end.
}
