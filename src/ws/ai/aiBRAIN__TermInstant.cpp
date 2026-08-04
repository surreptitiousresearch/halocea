#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiSTATUS.h"
#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/ai/aiCON_CB_globals.h"
#include "../../headers/ws/msg/msgSYSTEM.h"

// aiBRAIN::TermInstant @ 0x831776D0
// ?TermInstant@aiBRAIN@@MAAXXZ  (protected virtual)
//
// Immediate teardown, taken only while the brain is still active (stateAddr bit 0x200 clear):
// force-clear the mind, unregister the unit from the planner, post the terminate message (2) to
// self, set the status death flags, and flush the trash bin.
void aiBRAIN::TermInstant()
{
    if ((stateAddr.state & 0x200) != 0)
        return;

    mind.ForcedClear();
    aiPlanner->UnRegisterUnit(this);
    msgSystem.PostMsg(this, 2, this);

    aiSTATUS *pStatus = st.pointee;
    // Set the terminate/dead death-state bits (0x5), clearing 0x205's other bits per the disasm.
    pStatus->die.val = (pStatus->die.val & 0xFFFFFDFA) | 5;

    bin.Update();
}
