#include "../../headers/ws/ai/aiMIND.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiSTATUS.h"
#include "../../headers/ws/ai/aiENEMY_BASE.h"
#include "../../headers/ws/ai/aiPERC_BASE.h"
#include "../../headers/ws/ai/aiVISION_BASE.h"
#include "../../headers/ws/ai/aiSND_BASE.h"
#include "../../headers/ws/dsc/dscDESC.h"
#include "../../headers/ws/ai/aiBRAIN_DESC_ssl_boundary.h"
#include "../../headers/ws/ai/aiBRAIN_boundary.h" // _apLog

// aiMIND::ProcessINITLive @ 0x832291E0
// ?ProcessINITLive@aiMIND@@QAAXXZ
//
// Initialise the mind modules for a unit that spawned alive. Seed the status flag block from the
// species descriptor (force-setup + four per-species general-state flags), stamp the standing
// live-mind status bits, then run each module's per-spawn init hook.
void aiMIND::ProcessINITLive()
{
    aiBRAIN      *brain  = static_cast<aiBRAIN *>(pBrain);
    const dscDESC *desc  = brain->spDesc.pointee;
    aiSTATUS     *status = brain->st.pointee;

    if (aiBRAIN_DESC_ForceSetupInitial(desc))
    {
        status->gen.val |= 0x80u; // gen bit 7 — force initial setup
        _apLog("~AI~'%s': enabled force setup initial.", brain->names.nameInst);
    }

    if (aiBRAIN_DESC_InitGenFlagA(desc)) status->gen.val |= 0x10000u;
    if (aiBRAIN_DESC_InitGenFlagB(desc)) status->gen.val |= 0x20000u;
    if (aiBRAIN_DESC_InitGenFlagC(desc)) status->gen.val |= 0x40000u;
    if (aiBRAIN_DESC_InitGenFlagD(desc)) status->gen.val |= 0x80000u;

    status->mind.val |= 0x8000u;
    status->mind.val |= 0x10000000u;

    enemy.pointee->SetSelectionFlags();
    perc.pointee->ProcessInit();
    vision.pointee->ProcessInit();
    snd.pointee->ProcessInit();
}
