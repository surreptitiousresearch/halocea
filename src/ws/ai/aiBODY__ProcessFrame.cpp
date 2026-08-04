#include "../../headers/ws/ai/aiBODY.h"
#include "../../headers/ws/use/usePROCESSOR.h"      // usePROCESSOR::isLocked
#include "../../headers/ws/ai/aiBRAIN.h"            // aiBRAIN::st (dsSHARED_PTR<aiSTATUS>)
#include "../../headers/ws/ai/aiSTATUS.h"           // aiSTATUS::mind (dsFLAGS<AI_MIST,int>)

// aiBODY::ProcessFrame(float) @ 0x832AC540
// ?ProcessFrame@aiBODY@@UAAXM@Z
//
// Per-frame body step. Mirror the mind's "locked" state (AI_MIST bit 0x400000) into the use
// processor, run the base body step, then advance the uncontrolled/falling timers.
void aiBODY::ProcessFrame(float dt)
{
    if (useProcessor)
        useProcessor->isLocked = (pBrain->st.pointee->mind.val & 0x400000) != 0;

    aiBODY_IFACE::ProcessFrame(dt); // base body step (statically resolved)
    UpdateUncFall(dt);
}
