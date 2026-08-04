#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiBRAIN_DESC_sslfunc_boundary.h"
#include "../../headers/ws/dsc/dscDESC.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ssl/sslERROR.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsTSTRING.h"

// aiBRAIN::SetFloat(const dsTSTRING<char>&, float) @ 0x8324F5F8
// ?SetFloat@aiBRAIN@@QAAXABV?$dsTSTRING@D@@M@Z
//
// Run the descriptor's "SetFloat" SSL function with (name, value), marshalled into a contiguous
// dsDATA[2]; no return value. The disassembly passes a null retVal pointer to CallFunc.
void aiBRAIN::SetFloat(const dsTSTRING<char> &name, float value)
{
    dsDATA args[2];
    args[0].type = nullptr;
    args[0].StoreValue<dsTSTRING<char> >(name);
    args[1].type = nullptr;
    args[1].StoreValue<float>(value);

    // RAII: the returned sslERROR temporary and the empty caller ref self-release on return.
    sslObject.CallFunc(aiBRAIN_DESC_SslFnSetFloat(spDesc.pointee), 2, args, nullptr, sslOBJ_REF());
    // args[] destructors release their stored values at scope end.
}
