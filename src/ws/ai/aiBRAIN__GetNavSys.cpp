#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiBRAIN_DESC_sslfunc_boundary.h"
#include "../../headers/ws/dsc/dscDESC.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ssl/sslERROR.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/ds/dsDATA_SIMPLE_WRAPPER.h"

// aiBRAIN::GetNavSys() @ 0x83257910
// ?GetNavSys@aiBRAIN@@QAA?AV?$dsTSTRING@D@@XZ
//
// Run the descriptor's "GetNavSys" SSL function (no args); its return value is the name of the
// nav-system this brain is on. Unbox the returned dsDATA into a dsTSTRING<char> and return by value.
// NOTE: distinct from the virtual implGetNavSys() (0x83247F68) — this is the SSL-dispatch accessor.
dsTSTRING<char> aiBRAIN::GetNavSys()
{
    dsDATA ret;
    ret.type = nullptr;

    // RAII: the returned sslERROR temporary and the empty caller ref self-release on return.
    sslObject.CallFunc(aiBRAIN_DESC_SslFnGetNavSys(spDesc.pointee), 0, nullptr, &ret, sslOBJ_REF());

    dsDATA_SIMPLE_WRAPPER<dsTSTRING<char> > nav(ret, nullptr);
    return nav.val;
    // nav + ret destructors self-release at scope end.
}
