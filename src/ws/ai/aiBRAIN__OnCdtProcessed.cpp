#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiSTATUS.h"
#include "../../headers/ws/fsm/fsmPARAM_CDT.h"
#include "../../headers/ws/cdt/cdtINFO.h"

// 0x832ABE30 ?OnCdtProcessed@aiBRAIN@@UAAXPAUfsmPARAM_CDT@@@Z
// Fold a completed collision-detection query `par` into the status radar in/out targets and set/clear
// the "collision pending" body-flag (0x800) based on whether either the geometry or anim hit is valid.
void aiBRAIN::OnCdtProcessed(fsmPARAM_CDT *par)
{
    aiSTATUS *status = this->st.pointee;
    status->radar.trIn = par->trIn;
    status->radar.trOut = par->trOut;

    if (par->cdtGeom->IsValid() || par->cdtAnim->IsValid())
    {
        status->body.val |= 0x800;
        par->cdtGeom->IsValid(); // redundant re-check preserved from the binary
    }
    else
    {
        status->body.val &= ~0x800;
    }
}
