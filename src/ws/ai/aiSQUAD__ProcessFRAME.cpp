#include "../../headers/ws/ai/aiSQUAD.h"
#include "../../headers/ws/ai/aiDEBUG.h"

// aiSQUAD::ProcessFRAME() @ 0x832A8D88  (?ProcessFRAME@aiSQUAD@@MAAXXZ, virtual override)
// Per-frame squad update: run the base entity frame, then track the global off-AI debug switch,
// firing the OnDbgPause / OnDbgResume SSL events on transitions (state bit 0x2 = dbg-paused).
void aiSQUAD::ProcessFRAME()
{
    entENTITY::ProcessFRAME();

    bool offAI = aiDEBUG::IsOffAI() != 0;
    bool paused = ((state.val >> 1) & 1) != 0;

    if (offAI)
    {
        if (!paused)
        {
            state.val |= 2;
            OnDbgPause();
        }
    }
    else if (paused)
    {
        state.val &= ~2u;
        OnDbgResume();
    }
}
