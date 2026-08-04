#include "../../headers/ws/ai/aiWATCHER_PLAYER.h"
#include "../../headers/ws/ai/aiWATCHER_PLAYER_fsm_boundary.h" // propFSM/eventHandler + dbg-info-sys
#include "../../headers/ws/ai/aiWATCHER_BRAIN_boundaries.h"    // m3dVZero
#include "../../headers/ws/m3d/m3dV.h"

// aiWATCHER_PLAYER::GetBodyPosNav() const @ 0x83290698
// ?GetBodyPosNav@aiWATCHER_PLAYER@@UBA?AUm3dV@@XZ
// Virtual const (PPC sret — the decompiler mislabels the hidden return pointer as `this`): the
// nav-space body reference position. In the normal case this is bodyPos + 0.75*bodyUp (from
// GetBodyCS). In wall-lean (WLN) mode the position is pulled from the body FSM's debug info-system
// (surfaced through the dbg_get_info_sys event callback). m3dVZero when invalid or unavailable.
m3dV *aiWATCHER_PLAYER::GetBodyPosNav(m3dV *out) const
{
    aiWATCHER_PLAYER *self = const_cast<aiWATCHER_PLAYER *>(this);

    if (!self->IsValid())
    {
        *out = m3dVZero;
        return out;
    }

    if (!IsWlnMode())
    {
        m3dV pos, dir, up;
        GetBodyCS(pos, dir, up);
        m3dV nav = up;
        nav *= 0.75f;
        nav += pos;
        *out = nav;
        return out;
    }

    // Wall-lean: query the body FSM for the current body position via its debug info-system.
    fsmBODY_INFO_SYS_PARAM param;
    param.sys = nullptr;
    if (bodyFsm
        && bodyFsm->eventHandler.ActivateCB(fsmBODY_FSM_EVENT_events::dbg_get_info_sys(), &param)
        && param.sys)
    {
        unsigned char info[48];
        param.sys->GetDbgInfo(info); // vtbl+0x94; body world position lives at info+0x30
        *out = *reinterpret_cast<const m3dV *>(info + 0x30);
    }
    else
    {
        *out = m3dVZero;
    }
    return out;
}
