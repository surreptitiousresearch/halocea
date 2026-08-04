#include "../../headers/ws/ai/aiSQUAD.h"
#include "../../headers/ws/ai/aiSQUAD_DESC.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ssl/sslERROR.h"

// aiSQUAD::OnDbgPause() @ 0x832A8630  (?OnDbgPause@aiSQUAD@@QAAXXZ)
// Run the squad descriptor's "OnDbgPause" SSL function (no args, no return).
void aiSQUAD::OnDbgPause()
{
    aiSQUAD_DESC *desc = reinterpret_cast<aiSQUAD_DESC *>(spDesc.pointee);
    sslObject.CallFunc(desc->sslFuncId_OnDbgPause, 0, nullptr, nullptr, sslOBJ_REF());
}
