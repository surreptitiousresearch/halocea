#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiBRAIN_DESC_sslfunc_boundary.h"
#include "../../headers/ws/dsc/dscDESC.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ssl/sslERROR.h"

// aiBRAIN::tClear() @ 0x83251060
// ?tClear@aiBRAIN@@QAAXXZ
//
// Run the descriptor's "tClear" SSL function (no arguments, no return value).
void aiBRAIN::tClear()
{
    // RAII: the returned sslERROR temporary and the empty caller ref self-release on return.
    sslObject.CallFunc(aiBRAIN_DESC_SslFntClear(spDesc.pointee), 0, nullptr, nullptr, sslOBJ_REF());
}
