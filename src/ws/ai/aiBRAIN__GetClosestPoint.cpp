#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiBRAIN_DESC_sslfunc_boundary.h"
#include "../../headers/ws/dsc/dscDESC.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ssl/sslERROR.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/ds/dsDATA_SIMPLE_WRAPPER.h"

// aiBRAIN::GetClosestPoint(const dsTSTRING<char>&, const dsTSTRING<char>&) @ 0x83257E80
// ?GetClosestPoint@aiBRAIN@@QAA?AV?$dsTSTRING@D@@ABV2@0@Z
//
// Run the descriptor's "GetClosestPoint" SSL function with (points, filter), marshalled into a
// contiguous dsDATA[2]; unbox the returned point name back into a dsTSTRING<char> and return it.
dsTSTRING<char> aiBRAIN::GetClosestPoint(const dsTSTRING<char> &points, const dsTSTRING<char> &filter)
{
    dsDATA args[2];
    args[0].type = nullptr;
    args[0].StoreValue<dsTSTRING<char> >(points);
    args[1].type = nullptr;
    args[1].StoreValue<dsTSTRING<char> >(filter);

    dsDATA ret;
    ret.type = nullptr;

    // RAII: the returned sslERROR temporary and the empty caller ref self-release on return.
    sslObject.CallFunc(aiBRAIN_DESC_SslFnGetClosestPoint(spDesc.pointee), 2, args, &ret, sslOBJ_REF());

    dsDATA_SIMPLE_WRAPPER<dsTSTRING<char> > name(ret, nullptr);
    return name.val;
    // name, ret and args[] destructors self-release at scope end.
}
