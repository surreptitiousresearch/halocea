#include "../../headers/ws/ai/aiPROP_FIREABLE.h"
#include "../../headers/ws/ai/aiPROP_FIREABLE_boundaries.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiSTATUS.h"
#include "../../headers/ws/ai/aiBODY_IFACE.h"
#include "../../headers/ws/dmg/dmgDAMAGE.h"
#include "../../headers/ws/dmg/dmgSYSTEM.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include <string.h>

struct bhvBEHAV; // ai08 behaviour base (aiTRASH_BIN::Trash arg)  boundary (fwd)

// aiPROP_FIREABLE::Die() @ 0x832A3F88
// ?Die@aiPROP_FIREABLE@@UAAXABV?$dsTSTRING@D@@@Z  (vtbl)
//
// Scripted death driven by a kill-word sequence string. REMOVE/HIDE trash the unit outright; RAGDOLL
// (with a revitalize-die flag) revitalizes instead of dying; the death-mode words (PIERCE/MIND/KEEP/
// BURN/WATER) set the corresponding aiSTATUS die-flag. A large self-damage impulse is then applied to
// topple the body, and if the unit is invulnerable/god-mode the base death is forced.
void aiPROP_FIREABLE::Die(const dsTSTRING<char> &nameSeq)
{
    aiBRAIN *pOwner = (aiBRAIN *)this->pOwner;
    const char *word = nameSeq.CStr();

    if (!strcmp(word, "REMOVE"))
    {
        pOwner->bin.Trash((bhvBEHAV *)this->pOwner);
        return;
    }

    if (strcmp(word, "RAGDOLL") != 0)
    {
        if (strcmp(word, "HIDE") != 0)
        {
            unsigned int state = this->state.state;
            if (((state >> 1) & 1) != 0 || ((state >> 2) & 1) != 0)
                return; // already dead/dying
        }
        else
        {
            pOwner->bin.Trash((bhvBEHAV *)this->pOwner);
        }
    }
    else
    {
        unsigned int state = this->state.state;
        if ((((state >> 1) & 1) != 0 || ((state >> 2) & 1) != 0)
            && (((unsigned int)pOwner->st.pointee->die.val >> 1) & 1) != 0)
        {
            this->Revitalize();
            pOwner->st.pointee->die.val &= 0xFFFFFFFC;
            pOwner->st.pointee->die.val |= 0x1000000;
        }
        this->isSkipRevitalize = true;
    }

    pOwner->mind.ZeroISA();

    float healthCur = this->healthCur;
    const dmgTYPE *pDmgType = dmgSystem->GetDmgType(1u);
    dmgDAMAGE dmg(pDmgType, healthCur + 10.0f);

    aiSTATUS *pStatus = pOwner->st.pointee;
    if (strcmp(word, "PIERCE") != 0)
    {
        if (strcmp(word, "MIND") != 0)
        {
            if (strcmp(word, "KEEP") != 0)
            {
                if (strcmp(word, "BURN") != 0)
                {
                    if (strcmp(word, "WATER") != 0)
                    {
                        if (strcmp(word, "HIDE") != 0)
                        {
                            if (!strcmp(word, "RAGDOLL"))
                            {
                                dmg.amount = 2000000.0f;
                                aiBODY_IFACE *pBody = pOwner->body.pointee;
                                pBody->SendFsmEventByName_2(fsmRAGDOLL_EVENT_start());
                            }
                        }
                        else
                        {
                            pOwner->bin.Trash((bhvBEHAV *)pOwner);
                        }
                    }
                    else
                    {
                        pStatus->die.val |= 0x10u; // WATER
                    }
                }
                else
                {
                    pStatus->die.val |= 0x100u; // BURN
                }
            }
            else
            {
                pStatus->die.val |= 0x80u; // KEEP
            }
        }
        else
        {
            pStatus->die.val |= 0x20u; // MIND
        }
    }
    else
    {
        pStatus->die.val |= 0x40u; // PIERCE
    }

    // Randomized topple origin/direction and self-hit anchor (cdtINFO + gsGEOM_ANCHOR build).  boundary.
    aifireable_BuildSelfDamageAnchor(&dmg, pOwner);
    dmg.impulse = 50.0f;
    this->AcceptDamage(&dmg);

    if (this->IsInvulnerable() || this->IsGodMode())
        this->propFIREABLE::Die(nameSeq);
}
