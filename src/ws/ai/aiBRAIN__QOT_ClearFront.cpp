#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiBRAIN_DESC_sslfunc_boundary.h"
#include "../../headers/ws/dsc/dscDESC.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ssl/sslERROR.h"

// aiBRAIN::QOT_ClearFront() @ 0x83251930
// ?QOT_ClearFront@aiBRAIN@@QAAXXZ
//
// Run the descriptor's "QOT_ClearFront" SSL function (no args, no return value used).
void aiBRAIN::QOT_ClearFront()
{
    sslObject.CallFunc(aiBRAIN_DESC_SslFnQOT_ClearFront(spDesc.pointee), 0, nullptr, nullptr, sslOBJ_REF());
}
