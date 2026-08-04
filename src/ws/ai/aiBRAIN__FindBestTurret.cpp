#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiBRAIN_DESC_sslfunc_boundary.h"
#include "../../headers/ws/dsc/dscDESC.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ssl/sslERROR.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/ds/dsDATA_SIMPLE_WRAPPER.h"

// aiBRAIN::FindBestTurret(float, bool) @ 0x83258780
// ?FindBestTurret@aiBRAIN@@QAA?AV?$dsTSTRING@D@@M_N@Z
//
// Run the descriptor's "FindBestTurret" SSL function with (range, flag), marshalled into a
// contiguous dsDATA[2]; unbox the returned turret name back into a dsTSTRING<char> and return it.
dsTSTRING<char> aiBRAIN::FindBestTurret(float range, bool flag)
{
    dsDATA args[2];
    args[0].type = nullptr;
    args[0].StoreValue<float>(range);
    args[1].type = nullptr;
    args[1].StoreValue<bool>(flag);

    dsDATA ret;
    ret.type = nullptr;

    // RAII: the returned sslERROR temporary and the empty caller ref self-release on return.
    sslObject.CallFunc(aiBRAIN_DESC_SslFnFindBestTurret(spDesc.pointee), 2, args, &ret, sslOBJ_REF());

    dsDATA_SIMPLE_WRAPPER<dsTSTRING<char> > name(ret, nullptr);
    return name.val;
    // name, ret and args[] destructors self-release at scope end.
}
