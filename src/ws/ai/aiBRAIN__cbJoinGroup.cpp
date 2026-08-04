#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/ds/dsDATA_SIMPLE_WRAPPER.h"

// aiBRAIN::cbJoinGroup(int, dsDATA*, dsDATA&, sslOBJ_REF) @ 0x83258F70
// ?cbJoinGroup@aiBRAIN@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z
//
// SSL "JoinGroup" callback: unbox two strings (group name argv[0], group type argv[1]), forward to
// implJoinGroup, and box the returned bool into retVal. `caller` self-releases on return.
void aiBRAIN::cbJoinGroup(int /*argc*/, dsDATA *argv, dsDATA &retVal, sslOBJ_REF /*caller*/)
{
    dsDATA_SIMPLE_WRAPPER<dsTSTRING<char> > group(argv[0], nullptr);
    dsDATA_SIMPLE_WRAPPER<dsTSTRING<char> > type(argv[1], nullptr);
    bool result = implJoinGroup(group.val, type.val);

    dsDATA out;
    out.type = nullptr;
    out.StoreValue<bool>(result);
    retVal.StoreValue(out);
    // out + wrapper destructors self-release at scope end.
}
