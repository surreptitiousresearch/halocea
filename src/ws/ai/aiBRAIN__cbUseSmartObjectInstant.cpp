#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/ds/dsDATA_SIMPLE_WRAPPER.h"
#include "../../headers/ws/ds/dsCONVERTION_TYPE.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"

// aiBRAIN::cbUseSmartObjectInstant(int, dsDATA*, dsDATA&, sslOBJ_REF) @ 0x83258458
// ?cbUseSmartObjectInstant@aiBRAIN@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z
//
// SSL "UseSmartObjectInstant" callback: unbox (object, action, keepPlaying) out of argv and forward
// to the virtual implUseSmartObjectInstant; store its bool result into `retVal`.
void aiBRAIN::cbUseSmartObjectInstant(int /*argc*/, dsDATA *argv, dsDATA &retVal, sslOBJ_REF /*caller*/)
{
    dsDATA_SIMPLE_WRAPPER<dsTSTRING<char> > object(argv[0], nullptr);
    dsDATA_SIMPLE_WRAPPER<dsTSTRING<char> > action(argv[1], nullptr);
    bool keepPlaying = false;
    argv[2].GetValue<bool>(keepPlaying, DSD_CONV_RETRIEVE);

    bool result = implUseSmartObjectInstant(object.val, action.val, keepPlaying);
    retVal.StoreValue<bool>(result);
}
