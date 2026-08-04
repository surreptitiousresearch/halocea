#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/ds/dsDATA_SIMPLE_WRAPPER.h"
#include "../../headers/ws/ds/dsCONVERTION_TYPE.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"

// aiBRAIN::cbMoveToPoint(int, dsDATA*, dsDATA&, sslOBJ_REF) @ 0x832579F0
// ?cbMoveToPoint@aiBRAIN@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z
//
// SSL "MoveToPoint" callback: read the bool flag (argv[3]) and unbox three name strings
// (argv[0..2]), forward them to the virtual implMoveToPoint(argv0, argv1, argv2, flag) and box its
// bool result into `retVal`. `caller` self-releases on return.
void aiBRAIN::cbMoveToPoint(int /*argc*/, dsDATA *argv, dsDATA &retVal, sslOBJ_REF /*caller*/)
{
    bool flag = false;
    argv[3].GetValue<bool>(flag, DSD_CONV_RETRIEVE);

    dsDATA_SIMPLE_WRAPPER<dsTSTRING<char> > how(argv[2], nullptr);
    dsDATA_SIMPLE_WRAPPER<dsTSTRING<char> > point(argv[1], nullptr);
    dsDATA_SIMPLE_WRAPPER<dsTSTRING<char> > navSys(argv[0], nullptr);

    bool result = implMoveToPoint(navSys.val, point.val, how.val, flag);

    dsDATA out;
    out.type = nullptr;
    out.StoreValue<bool>(result);
    retVal.StoreValue(out);
}
