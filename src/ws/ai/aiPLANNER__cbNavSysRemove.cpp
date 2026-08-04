#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/ds/dsDATA_SIMPLE_WRAPPER.h"

// aiPLANNER::cbNavSysRemove(int, dsDATA*, dsDATA&, sslOBJ_REF) @ 0x8325A0F0
// ?cbNavSysRemove@aiPLANNER@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z
//
// SSL "NavSysRemove" callback: unbox argv[0] (nav-system name), forward to implNavSysRemove and store
// the returned bool into `retVal`. `caller` releases on return.
void aiPLANNER::cbNavSysRemove(int /*argc*/, dsDATA *argv, dsDATA &retVal, sslOBJ_REF /*caller*/)
{
    dsDATA_SIMPLE_WRAPPER<dsTSTRING<char> > nsName(argv[0], nullptr);

    bool res = implNavSysRemove(nsName.val);

    dsDATA out;
    out.type = nullptr;
    out.StoreValue<bool>(res);
    retVal.StoreValue(out);
    // wrapper + out destructors self-release at scope end.
}
