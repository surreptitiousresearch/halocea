#include "../../headers/ws/ai/aiLOD_SYS.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiSTATUS.h"

// aiLOD_SYS::MayChooseStrike @ 0x83239050
// ?MayChooseStrike@aiLOD_SYS@@QAA_NPBVaiBRAIN@@@Z
//
// May `pBrain` pick a new strike right now? Always yes for high-detail units (LOD < 10). Otherwise
// the decision is gated by the system-wide cached strike load budget (ceiling 20), with a stricter
// (0.7x) effective budget for mid-detail units (LOD 10..19) than for low-detail units (LOD >= 20).
bool aiLOD_SYS::MayChooseStrike(const aiBRAIN *pBrain)
{
    int value = pBrain->st.pointee->lod.value;
    if (value < 10)
        return true;
    if (value >= 20)
        return cachedLoadSum < 20.0f;
    return cachedLoadSum * 0.69999999f < 20.0f;
}
