// aiMIND::SetupScriptedEnemy @ 0x8322BFE0
// ?SetupScriptedEnemy@aiMIND@@QAAXPAVentTRACKER@@@Z
//
// Force the mind's enemy to the entity referenced by tracker `tr`:
//   - null tracker (or a tracker resolving to no entity): leave scripted mode (identical to the
//     StopScriptedEnemy body — clear enemy, drop the scripted bit, reset sparsings, normal selene);
//   - otherwise: set the scripted-enemy status bit and try to assign the entity as the enemy
//     (accepting only a live, real target via EnemySystemCheck). On success zero the selene; on
//     failure log an "~AI,Error~" and back the scripted bit out.
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiSTATUS.h"
#include "../../headers/ws/ai/aiWATCHER.h"
#include "../../headers/ws/ent/entTRACKER.h"
#include "../../headers/ws/ds/dsTSTRING.h"

extern void _apLog(const char *fmt, ...); // ?_apLog@@YAXPBDZZ — engine logger  boundary

namespace {
// `anonymous namespace'::EnemySystemCheck @ 0x83227BE0 — the acceptance predicate handed to
// InternalAssignEnemy: accept `cand` only when it watches a real (non-null) and non-dead target.
bool EnemySystemCheck(aiBRAIN * /*pBrain*/, aiWATCHER *cand)
{
    if (cand->Is(nullptr))
        return false;
    if (cand->IsDead())
        return false;
    return true;
}
} // namespace

void aiMIND::SetupScriptedEnemy(entTRACKER *tr)
{
    aiBRAIN *brain = static_cast<aiBRAIN *>(pBrain);

    if (!tr)
    {
        ZeroEnemy();
        brain->st.pointee->est.val &= ~0x80000;
        brain->ResetAllSparsings();
        SetNormalSelene();
        return;
    }

    entENTITY *ent = tr->holder.GetEnt();
    if (!ent)
        return;

    brain->st.pointee->est.val |= 0x80000; // enter scripted-enemy mode

    if (InternalAssignEnemy(ent, 1, EnemySystemCheck))
    {
        ZeroSelene();
    }
    else
    {
        dsTSTRING<char> entName = ent->GetName();
        _apLog("~AI,Error~'%s': can't SetEnemy('%s')", brain->names.nameInst, entName.CStr());
        brain->st.pointee->est.val &= ~0x80000; // back out of scripted mode
    }
}
