#include "../../headers/ws/ai/aiSQUAD.h"
#include "../../headers/ws/ai/aiSQUAD_DESC.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ssl/sslERROR.h"

// aiSQUAD::OnDbgResume() @ 0x832A86B0  (?OnDbgResume@aiSQUAD@@QAAXXZ)
// Run the squad descriptor's "OnDbgResume" SSL function (no args, no return).
void aiSQUAD::OnDbgResume()
{
    aiSQUAD_DESC *desc = reinterpret_cast<aiSQUAD_DESC *>(spDesc.pointee);
    sslObject.CallFunc(desc->sslFuncId_OnDbgResume, 0, nullptr, nullptr, sslOBJ_REF());
}
