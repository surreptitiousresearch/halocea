#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiSTATUS.h"
#include "../../headers/ws/dsc/dscDESC.h"

// aiBRAIN::ProcessINITAnyHealth @ 0x83178900
// ?ProcessINITAnyHealth@aiBRAIN@@UAAHXZ  (virtual)
//
// Initialise the brain independently of spawn health: grab its name strings, seed the general
// status flags (some copied from the descriptor), fill the combat sub-block from the instance,
// clear the mind's enemy, and reset the command block. Returns 1 (success).
int aiBRAIN::ProcessINITAnyHealth()
{
    names.Grab(this);
    st.pointee->gen.val |= 4u;
    stateEnt.val |= 0x80000u;

    aiSTATUS *pStatus = st.pointee;
    dscDESC  *pDesc = spDesc.pointee;

    // Three general-status flags are copied from bytes of the derived (brain) descriptor, past
    // dscDESC's own fields — the descriptor subclass layout is the next frontier, so read them by
    // verified byte offset.
    unsigned char *descBytes = reinterpret_cast<unsigned char *>(pDesc);

    if (descBytes[0x1F0]) pStatus->gen.val |= 0x10;     else pStatus->gen.val &= ~0x10;
    if (descBytes[0x1F5]) pStatus->gen.val |= 0x4000;   else pStatus->gen.val &= ~0x4000;
    if (descBytes[0x1F7]) pStatus->gen.val |= 0x100000; else pStatus->gen.val &= ~0x100000;

    pStatus->cs.FillInitial(pInst);
    mind.ZeroEnemy();
    cmd.FillDefault();
    return 1;
}
