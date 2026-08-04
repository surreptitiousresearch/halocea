#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiBRAIN_DESC_sslfunc_boundary.h"
#include "../../headers/ws/dsc/dscDESC.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ssl/sslERROR.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsTSTRING.h"

// aiBRAIN::SetApproach(const dsTSTRING<char>&) @ 0x8324C4C8
// ?SetApproach@aiBRAIN@@QAAXABV?$dsTSTRING@D@@@Z
//
// Run the descriptor's "SetApproach" SSL function with an approach name (no return value used).
void aiBRAIN::SetApproach(const dsTSTRING<char> &approach)
{
    dsDATA arg;
    arg.type = nullptr;
    arg.StoreValue<dsTSTRING<char> >(approach);

    // RAII: the returned sslERROR temporary and the empty caller ref self-release on return.
    sslObject.CallFunc(aiBRAIN_DESC_SslFnSetApproach(spDesc.pointee), 1, &arg, nullptr, sslOBJ_REF());
    // arg destructor releases its stored value at scope end.
}
