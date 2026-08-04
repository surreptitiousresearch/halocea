// aiPLANNER::ProcessDebugKeys @ 0x8319BCF8
#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/ai/aiPLANNER_debugkeys_boundaries.h" // gsSysInput, _aiConCb, UpdateOffAI,
                                                                // dbg_ai_PickCamPosForTrajCheck
#include "../../headers/ws/ai/aiCON_CB_globals.h"               // _pBrainDebugWatchee
#include "../../headers/ws/ai/aiWATCHER_BRAIN.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiTEAMS_SYS.h"                    // IsEnemyToPlayer
#include "../../headers/ws/prop/propGAME_INFO_TEAM.h"
#include "../../headers/ws/ds/WEAK_PTR.h"
#include "../../headers/ws/ds/REF_TYPE.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/dbg/dbgVAR_MANAGER.h"
#include "../../headers/ws/wb/dbgVAR_boundary.h"                // dbgVAR_IMPL<bool,1>

// Poll the AI debug-console key contexts once per frame and act on any that were just pressed.
void aiPLANNER::ProcessDebugKeys()
{
    // dbg_ai_clear_watchee — drop the currently-watched brain.
    {
        ds_data::REF_TYPE<gsiCONTEXT> ctx = gsSysInput->GetDbgContext();
        if (ctx->IsJustOn("dbg_ai_clear_watchee"))
        {
            _pBrainDebugWatchee = nullptr;
            if (_aiConCb)
                _aiConCb->SendWatcheeToConsole(false);
        }
    }

    // dbg_ai_onoff — toggle the "AI/OFF AI" debug bool var.
    {
        ds_data::REF_TYPE<gsiCONTEXT> ctx = gsSysInput->GetDbgContext();
        if (ctx->IsJustOn("dbg_ai_onoff"))
        {
            bool current = false;
            {
                dsTSTRING<char> name;
                name.UnsafeInit("AI/OFF AI", -1, 0);
                dbgVAR *var = dbgVAR_MANAGER::GetManager()->FindFirst(name);
                if (var && var->typeId == 1)
                    current = static_cast<dbgVAR_IMPL<bool, 1> *>(var)->value;
            }
            // The decompiler inlined the public SetDbgVar<bool,1> wrapper (lookup name -> if
            // typeId==N overwrite value and fire the private SendVarUpdate). Refold to that public
            // API rather than call the private SendVarUpdate directly.
            dsTSTRING<char> name2;
            name2.UnsafeInit("AI/OFF AI", -1, 0);
            bool toggled = !current;
            dbgVAR_MANAGER::GetManager()->SetDbgVar<bool, 1>(name2, toggled);
        }
    }

    // dbg_ai_kill_watchee — ragdoll-kill the watched brain.
    {
        ds_data::REF_TYPE<gsiCONTEXT> ctx = gsSysInput->GetDbgContext();
        bool fire = ctx->IsJustOn("dbg_ai_kill_watchee") && _pBrainDebugWatchee;
        if (fire)
        {
            dsTSTRING<char> reason;
            reason.UnsafeInit("RAGDOLL", -1, 0);
            _pBrainDebugWatchee->Die(reason);
        }
    }

    // dbg_ai_kill_all — ragdoll-kill every active unit.
    {
        ds_data::REF_TYPE<gsiCONTEXT> ctx = gsSysInput->GetDbgContext();
        if (ctx->IsJustOn("dbg_ai_kill_all"))
        {
            int i = 0;
            while (i < this->wah.arrUnits.nElem)
            {
                aiWATCHER_BRAIN *watcher = this->wah.PeekActive(i);
                dsTSTRING<char> reason;
                reason.UnsafeInit("RAGDOLL", -1, 0);
                ds::WEAK_PTR_HANDLE<aiBRAIN> *handle = watcher->brain.pHandle;
                aiBRAIN *brain = handle ? handle->pPtr : nullptr;
                brain->Die(reason);
                // Die may unregister the watcher; only advance if it's still the same active entry.
                if (i < this->wah.arrUnits.nElem && watcher == this->wah.PeekActive(i))
                    ++i;
            }
        }
    }

    // dbg_ai_kill_enemies — ragdoll-kill every active unit hostile to the player.
    {
        ds_data::REF_TYPE<gsiCONTEXT> ctx = gsSysInput->GetDbgContext();
        if (ctx->IsJustOn("dbg_ai_kill_enemies"))
        {
            int i = 0;
            while (i < this->wah.arrUnits.nElem)
            {
                aiWATCHER_BRAIN *watcher = this->wah.PeekActive(i);
                propGAME_INFO_TEAM *team = watcher->GetTeamID();
                if (((aiTEAMS_SYS *)&this->teams)->IsEnemyToPlayer(*team))
                {
                    dsTSTRING<char> reason;
                    reason.UnsafeInit("RAGDOLL", -1, 0);
                    ds::WEAK_PTR_HANDLE<aiBRAIN> *handle = watcher->brain.pHandle;
                    aiBRAIN *brain = handle ? handle->pPtr : nullptr;
                    brain->Die(reason);
                }
                if (i < this->wah.arrUnits.nElem && watcher == this->wah.PeekActive(i))
                    ++i;
            }
        }
    }

    // dbg_ai_pick_campos_for_traj_check — arm the one-shot trajectory-checker camera pick.
    {
        ds_data::REF_TYPE<gsiCONTEXT> ctx = gsSysInput->GetDbgContext();
        if (ctx->IsJustOn("dbg_ai_pick_campos_for_traj_check"))
            dbg_ai_PickCamPosForTrajCheck = true;
    }

    // dbg_ai_create_ally_mate — spawn a debug "ally_mate" AI entity.
    {
        ds_data::REF_TYPE<gsiCONTEXT> ctx = gsSysInput->GetDbgContext();
        if (ctx->IsJustOn("dbg_ai_create_ally_mate"))
        {
            dsTSTRING<char> emptyName;
            emptyName.UnsafeInit("", -1, 0);
            dsTSTRING<char> type;
            type.UnsafeInit("ally_mate", -1, 0);
            DebugCreateAIEntity(type, emptyName);
        }
    }

    UpdateOffAI();
}
