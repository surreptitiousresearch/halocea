#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/ds/dsDATA_SIMPLE_WRAPPER.h"

// aiPLANNER::cbEnableStrike(int, dsDATA*, dsDATA&, sslOBJ_REF) @ 0x8325A5C0
// ?cbEnableStrike@aiPLANNER@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z
//
// SSL "EnableStrike" callback: unbox argv[0] (strike name) and forward to implEnableStrike (no return
// value marshalled). `caller` releases on return.
void aiPLANNER::cbEnableStrike(int /*argc*/, dsDATA *argv, dsDATA & /*retVal*/, sslOBJ_REF /*caller*/)
{
    dsDATA_SIMPLE_WRAPPER<dsTSTRING<char> > nameStrike(argv[0], nullptr);
    implEnableStrike(nameStrike.val);
    // wrapper destructor self-releases at scope end.
}
