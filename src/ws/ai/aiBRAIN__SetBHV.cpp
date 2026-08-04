#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiBRAIN_DESC_sslfunc_boundary.h"
#include "../../headers/ws/dsc/dscDESC.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ssl/sslERROR.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsTSTRING.h"

// aiBRAIN::SetBHV(const dsTSTRING<char>&) @ 0x8324BA68
// ?SetBHV@aiBRAIN@@QAAXABV?$dsTSTRING@D@@@Z
//
// Run the descriptor's "SetBHV" SSL function with a behaviour name (no return value used). The name
// is copied into a local dsTSTRING (ref-counted), marshalled into a dsDATA, then dispatched.
void aiBRAIN::SetBHV(const dsTSTRING<char> &bhvName)
{
    dsTSTRING<char> name = bhvName;

    dsDATA argData;
    argData.type = nullptr;
    argData.StoreValue<dsTSTRING<char> >(name);

    // RAII: the returned sslERROR temporary and the empty caller ref self-release on return.
    sslObject.CallFunc(aiBRAIN_DESC_SslFnSetBHV(spDesc.pointee), 1, &argData, nullptr, sslOBJ_REF());
    // argData and name destructors release their stored values / buffer refs at scope end.
}
