#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiBRAIN_DESC_sslfunc_boundary.h"
#include "../../headers/ws/dsc/dscDESC.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ssl/sslERROR.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsTSTRING.h"

// aiBRAIN::SetNavSys(const dsTSTRING<char>&) @ 0x8324D198
// ?SetNavSys@aiBRAIN@@QAAXABV?$dsTSTRING@D@@@Z
//
// Run the descriptor's "SetNavSys" SSL function with nav-system name `name` as its single argument
// (no return value used).
void aiBRAIN::SetNavSys(const dsTSTRING<char> &name)
{
    dsDATA arg;
    arg.type = nullptr;
    arg.StoreValue<dsTSTRING<char> >(name);

    // RAII: the returned sslERROR temporary and the empty caller ref self-release on return.
    sslObject.CallFunc(aiBRAIN_DESC_SslFnSetNavSys(spDesc.pointee), 1, &arg, nullptr, sslOBJ_REF());
    // arg destructor releases its stored value at scope end.
}
