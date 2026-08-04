#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/ds/dsDATA_SIMPLE_WRAPPER.h"

// aiPLANNER::cbNavBlockObj(int, dsDATA*, dsDATA&, sslOBJ_REF) @ 0x83259A00
// ?cbNavBlockObj@aiPLANNER@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z
//
// SSL "NavBlockObj" callback: unbox two strings (scene-instance argv[0], sub-object argv[1]),
// forward to implNavBlockObj, box the returned bool into retVal. `caller` self-releases on return.
void aiPLANNER::cbNavBlockObj(int /*argc*/, dsDATA *argv, dsDATA &retVal, sslOBJ_REF /*caller*/)
{
    dsDATA_SIMPLE_WRAPPER<dsTSTRING<char> > instName(argv[0], nullptr);
    dsDATA_SIMPLE_WRAPPER<dsTSTRING<char> > objName(argv[1], nullptr);
    bool blocked = implNavBlockObj(instName.val, objName.val);

    dsDATA out;
    out.type = nullptr;
    out.StoreValue<bool>(blocked);
    retVal.StoreValue(out);
    // out + wrapper destructors self-release at scope end.
}
