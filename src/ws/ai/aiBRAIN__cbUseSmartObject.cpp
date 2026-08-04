#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/ds/dsDATA_SIMPLE_WRAPPER.h"
#include "../../headers/ws/ds/dsCONVERTION_TYPE.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"

// aiBRAIN::cbUseSmartObject(int, dsDATA*, dsDATA&, sslOBJ_REF) @ 0x832582D0
// ?cbUseSmartObject@aiBRAIN@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z
//
// SSL "UseSmartObject" callback: unbox (object, action, anim, instant, exitAction) out of argv and
// forward to the virtual implUseSmartObject; store its bool result into `retVal`.
void aiBRAIN::cbUseSmartObject(int /*argc*/, dsDATA *argv, dsDATA &retVal, sslOBJ_REF /*caller*/)
{
    dsDATA_SIMPLE_WRAPPER<dsTSTRING<char> > object(argv[0], nullptr);
    dsDATA_SIMPLE_WRAPPER<dsTSTRING<char> > action(argv[1], nullptr);
    dsDATA_SIMPLE_WRAPPER<dsTSTRING<char> > anim(argv[2], nullptr);
    bool instant = false;
    argv[3].GetValue<bool>(instant, DSD_CONV_RETRIEVE);
    dsDATA_SIMPLE_WRAPPER<dsTSTRING<char> > exitAction(argv[4], nullptr);

    bool result = implUseSmartObject(object.val, action.val, anim.val, instant, exitAction.val);
    retVal.StoreValue<bool>(result);
}
