#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/ds/dsDATA_SIMPLE_WRAPPER.h"
#include "../../headers/ws/ds/dsCONVERTION_TYPE.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"

// aiBRAIN::cbUseTurret(int, dsDATA*, dsDATA&, sslOBJ_REF) @ 0x83258660
// ?cbUseTurret@aiBRAIN@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z
//
// SSL "UseTurret" callback: unbox (turret, action, instant, keep) out of argv and forward to the
// virtual implUseTurret; store its bool result into `retVal`. `caller` self-releases on return.
void aiBRAIN::cbUseTurret(int /*argc*/, dsDATA *argv, dsDATA &retVal, sslOBJ_REF /*caller*/)
{
    dsDATA_SIMPLE_WRAPPER<dsTSTRING<char> > turret(argv[0], nullptr);
    dsDATA_SIMPLE_WRAPPER<dsTSTRING<char> > action(argv[1], nullptr);
    bool instant = false;
    argv[2].GetValue<bool>(instant, DSD_CONV_RETRIEVE);
    bool keep = false;
    argv[3].GetValue<bool>(keep, DSD_CONV_RETRIEVE);

    bool result = implUseTurret(turret.val, action.val, instant, keep);
    retVal.StoreValue<bool>(result);
}
