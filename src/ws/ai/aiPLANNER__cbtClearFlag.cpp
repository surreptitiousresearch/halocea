#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/ds/dsDATA_SIMPLE_WRAPPER.h"

// aiPLANNER::cbtClearFlag(int, dsDATA*, dsDATA&, sslOBJ_REF) @ 0x8325A788
// ?cbtClearFlag@aiPLANNER@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z
//
// SSL "tClearFlag" callback: unbox argv[0] (team-flag name), forward to impltClearFlag and store the
// returned bool into `retVal`. `caller` releases on return.
void aiPLANNER::cbtClearFlag(int /*argc*/, dsDATA *argv, dsDATA &retVal, sslOBJ_REF /*caller*/)
{
    dsDATA_SIMPLE_WRAPPER<dsTSTRING<char> > flag(argv[0], nullptr);

    bool res = impltClearFlag(flag.val);

    dsDATA out;
    out.type = nullptr;
    out.StoreValue<bool>(res);
    retVal.StoreValue(out);
    // wrapper + out destructors self-release at scope end.
}
