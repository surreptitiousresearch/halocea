#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/ds/dsDATA_SIMPLE_WRAPPER.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"

// aiBRAIN::cbUseSmartIdleInstant(int, dsDATA*, dsDATA&, sslOBJ_REF) @ 0x83258920
// ?cbUseSmartIdleInstant@aiBRAIN@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z
//
// SSL "UseSmartIdleInstant" callback: unbox (object, action) out of argv and forward to the virtual
// implUseSmartIdleInstant; store its bool result into `retVal`. `caller` self-releases on return.
void aiBRAIN::cbUseSmartIdleInstant(int /*argc*/, dsDATA *argv, dsDATA &retVal, sslOBJ_REF /*caller*/)
{
    dsDATA_SIMPLE_WRAPPER<dsTSTRING<char> > object(argv[0], nullptr);
    dsDATA_SIMPLE_WRAPPER<dsTSTRING<char> > action(argv[1], nullptr);

    bool result = implUseSmartIdleInstant(object.val, action.val);
    retVal.StoreValue<bool>(result);
}
