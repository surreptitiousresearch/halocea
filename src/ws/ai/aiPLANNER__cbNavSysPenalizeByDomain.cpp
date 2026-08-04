#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/ds/dsDATA_SIMPLE_WRAPPER.h"

// aiPLANNER::cbNavSysPenalizeByDomain(int, dsDATA*, dsDATA&, sslOBJ_REF) @ 0x83259FE0
// ?cbNavSysPenalizeByDomain@aiPLANNER@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z
//
// SSL "NavSysPenalizeByDomain" callback: unbox two strings (nav-system argv[0], domain argv[1]) and
// an int penalty argv[2], forward to implNavSysPenalizeByDomain, box the returned bool into retVal.
// `caller` self-releases on return.
void aiPLANNER::cbNavSysPenalizeByDomain(int /*argc*/, dsDATA *argv, dsDATA &retVal, sslOBJ_REF /*caller*/)
{
    int penalty;
    argv[2].GetValue<int>(penalty, DSD_CONV_RETRIEVE);
    dsDATA_SIMPLE_WRAPPER<dsTSTRING<char> > nameNS(argv[0], nullptr);
    dsDATA_SIMPLE_WRAPPER<dsTSTRING<char> > nameDom(argv[1], nullptr);
    bool penalized = implNavSysPenalizeByDomain(nameNS.val, nameDom.val, penalty);

    dsDATA out;
    out.type = nullptr;
    out.StoreValue<bool>(penalized);
    retVal.StoreValue(out);
    // out + wrapper destructors self-release at scope end.
}
