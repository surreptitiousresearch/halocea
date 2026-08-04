#include "../../headers/ws/strm/strmRES_TEX_MANAGER.h"
#include "../../headers/ws/strm/strmVID_MEM_MANAGER.h"
#include "../../headers/ws/strm/strm_boundary.h"
#include "../../headers/ws/txm/txmTEXTURE.h"

extern strmVID_MEM_MANAGER strmMemMng; // ?strmMemMng@@3VstrmVID_MEM_MANAGER@@A — global instance

// strmRES_TEX_MANAGER::NecessaryStreamTexture @ 0x826718C0
// Per-frame entry point for one live texture: reserve video memory, mark the owning texture as
// used-in-render, and drive its mip range via UpdateResource2 against the current total memory
// budget (the same &totalMem slot is deliberately reused for the totalMem/memHi/memMid/memLo
// budget-pointer arguments — DB/disasm-confirmed at the UpdateResource2 call site, not a
// decompiler artifact). While UpdateResource2 reports the mip range is still settling
// (RES_STATE_LOADING/RES_STATE_IN_QUERY), spin on osSleep(0); once loading completes
// (RES_STATE_READY) swap in the new surface and drop back to idle.
void strmRES_TEX_MANAGER::NecessaryStreamTexture(txmTEXTURE *pTex)
{
    strmMemMng.ReserveAllMemory();

    strmRESOURCE_TEXTURE *strmResource = pTex->strmResource;
    pTex->MarkAsUsedInRender();

    int totalMem = strmMemMng.GetSizeTotal();
    int rest = 0x80000;

    if (!UpdateResource2(strmResource, &totalMem, &totalMem, &totalMem, &totalMem, &rest)) {
        for (;;) {
            if (strmResource->state == strmRESOURCE::RES_STATE_READY) {
                pTex->Swap();
                strmResource->SetState(strmRESOURCE::RES_STATE_IDLE);
            }

            int state = strmResource->state;
            if (state != strmRESOURCE::RES_STATE_LOADING && state != strmRESOURCE::RES_STATE_IN_QUERY)
                break;

            osSleep(0);
        }
    }
}
