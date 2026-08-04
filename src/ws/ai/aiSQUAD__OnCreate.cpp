#include "../../headers/ws/ai/aiSQUAD.h"
#include "../../headers/ws/ai/aiSQUAD_DESC.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ssl/sslERROR.h"

// aiSQUAD::OnCreate() @ 0x832A85B0  (?OnCreate@aiSQUAD@@QAAXXZ)
// Run the squad descriptor's "OnCreate" SSL function (no args, no return). The returned sslERROR
// temporary and the empty caller ref self-release on return (the manual --refcount/dlFree in the
// disassembly is the sslOBJ_REF caller destructor).
void aiSQUAD::OnCreate()
{
    aiSQUAD_DESC *desc = reinterpret_cast<aiSQUAD_DESC *>(spDesc.pointee);
    sslObject.CallFunc(desc->sslFuncId_OnCreate, 0, nullptr, nullptr, sslOBJ_REF());
}
