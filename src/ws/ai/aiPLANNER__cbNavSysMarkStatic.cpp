#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/ds/dsDATA_SIMPLE_WRAPPER.h"

// aiPLANNER::cbNavSysMarkStatic(int, dsDATA*, dsDATA&, sslOBJ_REF) @ 0x83259F10
// ?cbNavSysMarkStatic@aiPLANNER@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z
//
// SSL "NavSysMarkStatic" callback: unbox the nav-system name string argv[0] and a bool argv[1],
// forward to implNavSysMarkStatic, box the returned bool into retVal. `caller` self-releases.
void aiPLANNER::cbNavSysMarkStatic(int /*argc*/, dsDATA *argv, dsDATA &retVal, sslOBJ_REF /*caller*/)
{
    bool isStatic;
    argv[1].GetValue<bool>(isStatic, DSD_CONV_RETRIEVE);
    dsDATA_SIMPLE_WRAPPER<dsTSTRING<char> > nsName(argv[0], nullptr);
    bool marked = implNavSysMarkStatic(nsName.val, isStatic);

    dsDATA out;
    out.type = nullptr;
    out.StoreValue<bool>(marked);
    retVal.StoreValue(out);
    // out + wrapper destructor self-release at scope end.
}
