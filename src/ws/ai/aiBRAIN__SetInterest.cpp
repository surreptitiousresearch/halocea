#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiBRAIN_DESC_sslfunc_boundary.h"
#include "../../headers/ws/dsc/dscDESC.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ssl/sslERROR.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsTSTRING.h"

// aiBRAIN::SetInterest(const dsTSTRING<char>&, const dsTSTRING<char>&, bool) @ 0x83250428
// ?SetInterest@aiBRAIN@@QAAXABV?$dsTSTRING@D@@0_N@Z
//
// Run the descriptor's "SetInterest" SSL function with (subject, kind, enable); no return value. The
// three arguments are marshalled into a contiguous dsDATA[3].
void aiBRAIN::SetInterest(const dsTSTRING<char> &subject, const dsTSTRING<char> &kind, bool enable)
{
    dsDATA args[3];
    args[0].type = nullptr;
    args[0].StoreValue<dsTSTRING<char> >(subject);
    args[1].type = nullptr;
    args[1].StoreValue<dsTSTRING<char> >(kind);
    args[2].type = nullptr;
    args[2].StoreValue<bool>(enable);

    // RAII: the returned sslERROR temporary and the empty caller ref self-release on return.
    sslObject.CallFunc(aiBRAIN_DESC_SslFnSetInterest(spDesc.pointee), 3, args, nullptr, sslOBJ_REF());
    // args[] destructors release their stored values at scope end.
}
