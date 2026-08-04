#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiBRAIN_DESC_sslfunc_boundary.h"
#include "../../headers/ws/dsc/dscDESC.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ssl/sslERROR.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsTSTRING.h"

// aiBRAIN::SetViewUpCurve(const dsTSTRING<char>&) @ 0x8324C8A8
// ?SetViewUpCurve@aiBRAIN@@QAAXABV?$dsTSTRING@D@@@Z
//
// Run the descriptor's "SetViewUpCurve" SSL function with a curve name (no return value used).
void aiBRAIN::SetViewUpCurve(const dsTSTRING<char> &curve)
{
    dsDATA arg;
    arg.type = nullptr;
    arg.StoreValue<dsTSTRING<char> >(curve);

    // RAII: the returned sslERROR temporary and the empty caller ref self-release on return.
    sslObject.CallFunc(aiBRAIN_DESC_SslFnSetViewUpCurve(spDesc.pointee), 1, &arg, nullptr, sslOBJ_REF());
    // arg destructor releases its stored value at scope end.
}
