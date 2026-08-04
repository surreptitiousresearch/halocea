#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiBRAIN_DESC.h"
#include "../../headers/ws/ai/ai_setup_subsystems_boundary.h"

// aiBRAIN::SetupMind @ 0x8317FB00
// ?SetupMind@aiBRAIN@@MAAXXZ  (protected virtual)
//
// Build the brain's mind modules (sound / task / goal / best / perception / interop / aim /
// approach / vision / selene) from the descriptor-driven subsystem-type selectors. Each module is
// installed only when its shared pointer is still empty, so re-entry is idempotent. Finally the
// enemy module, if unset, is zeroed. Selector words are packed in the descriptor (this+0x54
// pointee); the disasm-verified byte offset of each is noted inline.
void aiBRAIN::SetupMind()
{
    // Descriptor object (iaIACTOR::spDesc.pointee at this+0x54), viewed as the typed brain descriptor.
    const aiBRAIN_DESC *pDesc = *reinterpret_cast<const aiBRAIN_DESC *const *>(
        reinterpret_cast<const char *>(this) + 0x54);
    aiMIND *mind = &this->mind;

    // sound — NULL(1) / FULL(4)
    if (!mind->snd.pointee)
    {
        int t = pDesc->ssTypeSnd;
        if (t == 1)      mind->snd.Reset<aiSND_ZERO>(new aiSND_ZERO(this));
        else if (t == 4) mind->snd.Reset<aiSND>(new aiSND(this));
        else _apLog("~AI,Error~Wrong mind.snd subsystem type (not NULL / FULL).");
    }

    // task — NULL(1) / FULL(4)
    if (!mind->task.pointee)
    {
        int t = pDesc->ssTypeTask;
        if (t == 1)      mind->task.Reset<aiTASK_ZERO>(new aiTASK_ZERO(this));
        else if (t == 4) mind->task.Reset<aiTASK>(new aiTASK(this));
        else _apLog("~AI,Error~Wrong mind.task subsystem type (not NULL / FULL).");
    }

    // goal — NULL(1) / FULL(4)
    if (!mind->goal.pointee)
    {
        int t = pDesc->ssTypeGoal;
        if (t == 1)      mind->goal.Reset<aiGOAL_ZERO>(new aiGOAL_ZERO(this));
        else if (t == 4) mind->goal.Reset<aiGOAL>(new aiGOAL(this));
        else _apLog("~AI,Error~Wrong mind.goal subsystem type (not NULL / FULL).");
    }

    // best — NULL(1) / FULL(4)
    if (!mind->best.pointee)
    {
        int t = pDesc->ssTypeBest;
        if (t == 1)      mind->best.Reset<aiBEST_ZERO>(new aiBEST_ZERO(this));
        else if (t == 4) mind->best.Reset<aiBEST>(new aiBEST(this));
        else _apLog("~AI,Error~Wrong mind.best subsystem type (not NULL / FULL).");
    }

    // perception — NULL(1) / FULL(4)
    if (!mind->perc.pointee)
    {
        int t = pDesc->ssTypePerc;
        if (t == 1)      mind->perc.Reset<aiPERC_ZERO>(new aiPERC_ZERO(this));
        else if (t == 4) mind->perc.Reset<aiPERC>(new aiPERC(this));
        else _apLog("~AI,Error~Wrong mind.perc subsystem type (not NULL / FULL).");
    }

    // interop/social awareness — NULL(1) / FULL(4)
    if (!mind->isa.pointee)
    {
        int t = pDesc->ssTypeIsa;
        if (t == 1)      mind->isa.Reset<aiISA_ZERO>(new aiISA_ZERO(this));
        else if (t == 4) mind->isa.Reset<aiISA>(new aiISA(this));
        else _apLog("~AI,Error~Wrong mind.isa subsystem type (not NULL / FULL).");
    }

    // aim — NULL(1) / IDLE(2) / BATTLE(3)
    if (!mind->aim.pointee)
    {
        int t = pDesc->ssTypeAim;
        if (t == 1)      mind->aim.Reset<aiAIM_ZERO>(new aiAIM_ZERO(this));
        else if (t == 2) mind->aim.Reset<aiAIM_IDLE>(new aiAIM_IDLE(this));
        else if (t == 3) mind->aim.Reset<aiAIM_BATTLE>(new aiAIM_BATTLE(this));
        else _apLog("~AI,Error~Wrong mind.aim subsystem type (not NULL / IDLE / BATTLE).");
    }

    // approach — NULL(1) / FULL(4)
    if (!mind->aprog.pointee)
    {
        int t = pDesc->ssTypeAprog;
        if (t == 1)      mind->aprog.Reset<aiAPPROACH_ZERO>(new aiAPPROACH_ZERO(this));
        else if (t == 4) mind->aprog.Reset<aiAPPROACH>(new aiAPPROACH(this));
        else _apLog("~AI,Error~Wrong mind.aprog subsystem type (not NULL / FULL).");
    }

    // vision — NULL(1) / IDLE(2) / BATTLE(3)
    if (!mind->vision.pointee)
    {
        int t = pDesc->ssTypeVision;
        switch (t)
        {
        case 1: mind->vision.Reset<aiVISION_ZERO>(new aiVISION_ZERO(this)); break;
        case 2: mind->vision.Reset<aiVISION_IDLE>(new aiVISION_IDLE(this)); break;
        case 3: mind->vision.Reset<aiVISION_BATTLE>(new aiVISION_BATTLE(this)); break;
        default: _apLog("~AI,Error~Wrong mind.vision subsystem type (not NULL / IDLE / BATTLE).");
        }
    }

    // selene — NULL(1) / FULL(4)
    if (!mind->selene.pointee)
    {
        int t = pDesc->ssTypeSelene;
        if (t == 1)      mind->selene.Reset<aiSELENE_ZERO>(new aiSELENE_ZERO(this));
        else if (t == 4) mind->selene.Reset<aiSELENE>(new aiSELENE(this));
        else _apLog("~AI,Error~Wrong mind.selene subsystem type (not NULL / FULL).");
    }

    if (!mind->enemy.pointee)
        mind->ZeroEnemy();
}
