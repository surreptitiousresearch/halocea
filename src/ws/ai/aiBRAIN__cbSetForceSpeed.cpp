#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/ds/dsDATA_SIMPLE_WRAPPER.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"

// aiBRAIN::cbSetForceSpeed(int, dsDATA*, dsDATA&, sslOBJ_REF) @ 0x83258218
// ?cbSetForceSpeed@aiBRAIN@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z
//
// SSL "SetForceSpeed" callback: unbox the speed-preset name out of argv[0] and forward to the virtual
// implSetForceSpeed; store its bool result into `retVal`. `caller` self-releases on return.
void aiBRAIN::cbSetForceSpeed(int /*argc*/, dsDATA *argv, dsDATA &retVal, sslOBJ_REF /*caller*/)
{
    dsDATA_SIMPLE_WRAPPER<dsTSTRING<char> > speed(argv[0], nullptr);

    bool result = implSetForceSpeed(speed.val);
    retVal.StoreValue<bool>(result);
}
