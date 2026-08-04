#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/ds/dsDATA_SIMPLE_WRAPPER.h"
#include "../../headers/ws/ds/dsCONVERTION_TYPE.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"

// aiBRAIN::cbMoveToNavSys(int, dsDATA*, dsDATA&, sslOBJ_REF) @ 0x83257C80
// ?cbMoveToNavSys@aiBRAIN@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z
//
// SSL "MoveToNavSys" callback: unbox (navSys, arrive, style, run) out of argv and forward to the
// virtual implMoveToNavSys; store its bool result into `retVal`. `caller` self-releases on return.
void aiBRAIN::cbMoveToNavSys(int /*argc*/, dsDATA *argv, dsDATA &retVal, sslOBJ_REF /*caller*/)
{
    dsDATA_SIMPLE_WRAPPER<dsTSTRING<char> > navSys(argv[0], nullptr);
    dsDATA_SIMPLE_WRAPPER<dsTSTRING<char> > arrive(argv[1], nullptr);
    dsDATA_SIMPLE_WRAPPER<dsTSTRING<char> > style(argv[2], nullptr);
    bool run = false;
    argv[3].GetValue<bool>(run, DSD_CONV_RETRIEVE);

    bool result = implMoveToNavSys(navSys.val, arrive.val, style.val, run);
    retVal.StoreValue<bool>(result);
}
