#include "../../headers/ws/ai/aiLOD_SYS.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiSTATUS.h"
#include "../../headers/ws/ai/aiDEBUG_boundaries.h" // dbg_isOffMindSharing

// aiLOD_SYS::ShouldSkipMindFrame @ 0x83239328
// ?ShouldSkipMindFrame@aiLOD_SYS@@QAA_NPBVaiBRAIN@@@Z
//
// Should `pBrain`'s mind (think) frame be skipped this tick? Only when mind-sharing is enabled (the
// debug override is off), the brain is flagged sparsable (aiSTATUS gen bit 0x10), and it is NOT the
// single brain the sparser currently permits.
int aiLOD_SYS::ShouldSkipMindFrame(const aiBRAIN *pBrain)
{
    return !dbg_isOffMindSharing.value
        && ((pBrain->st.pointee->gen.val >> 4) & 1) != 0
        && msAllowed.pb != pBrain;
}
