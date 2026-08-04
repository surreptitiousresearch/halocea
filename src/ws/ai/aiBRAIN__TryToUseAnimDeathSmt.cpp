#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/ai/aiCON_CB_globals.h" // extern aiPLANNER *aiPlanner
#include "../../headers/ws/ai/aiBODY_IFACE.h"
#include "../../headers/ws/ai/aiBEST_BASE.h"
#include "../../headers/ws/ai/aiSTATUS.h"
#include "../../headers/ws/smt/smtSMART_OBJ_BASE.h"
#include "../../headers/ws/smt/smtSMT_ENTER_INFO.h"
#include "../../headers/ws/bhp/bhpSMART_PAR.h"
#include "../../headers/ws/ent/entTRACKER.h"
#include "../../headers/ws/gm/gmPLACE_CHECKER.h"
#include "../../headers/ws/ds/dsVECTOR.h"
#include "../../headers/ws/ds/dsSHARED_PTR.h"
#include "../../headers/ws/ds/dsPAIR.h"
#include "../../headers/ws/ds/dsSTRID.h"
#include "../../headers/ws/m3d/m3dV.h"
#include "../../headers/apCL.h"
#include <string.h>

// aiBRAIN::TryToUseAnimDeathSmt() @ 0x832698E8
// ?TryToUseAnimDeathSmt@aiBRAIN@@...
//
// When the descriptor allows animated-death smart objects and the status has a valid max distance,
// scan every registered anim-death smart object, find the nearest usable entry point (tracker set +
// valid + within max-dist^2), then push an "AnimDeathSmt" SMART behaviour toward it, flag the death
// state (die bit 0x2000) and re-activate the body callback. Returns true when a behaviour was set.

extern void _apLog(const char *fmt, ...);
extern "C" void dlFree(void *ptr);

// bhpPARAMS base vtable — the binary re-points bhpSMART_PAR's vtable to it during teardown.
extern bhpPARAMS_vtbl bhpPARAMS_vftable;

// fsmFSM_EVENT::reset_() — the interned "reset" FSM event id. boundary (static, defined in fsm).
struct fsmFSM_EVENT { static const dsSTRID *reset_(); };

static const char *const kSmartCpp =
    "D:\\Projects\\code\\common\\src.sys\\gm_shared\\ai08\\b8_smart.cpp";

bool aiBRAIN::TryToUseAnimDeathSmt()
{
    // Descriptor gate: "use anim-death smart" flag == HIBYTE(spDesc.pointee[57].pBrand).
    if (!(unsigned char)((unsigned int)(uintptr_t)this->spDesc.pointee[57].pBrand >> 24))
        return false;

    aiSTATUS *status = this->st.pointee;
    if (status->smtDeathMaxDist < 0.0f)
        return false;

    float bestDist2 = status->smtDeathMaxDist * status->smtDeathMaxDist;

    dsSHARED_PTR<smtSMART_OBJ_BASE, 0, Deleter<smtSMART_OBJ_BASE> > best; // null
    dsVECTOR<dsSHARED_PTR<smtSMART_OBJ_BASE, 0, Deleter<smtSMART_OBJ_BASE> >, 8> &deathObjs =
        aiPlanner->animDeathSmtObjects;
    int bestSlot = -1;

    int nObjs = deathObjs.nElem;
    for (int oi = 0; oi < nObjs; ++oi)
    {
        dsSHARED_PTR<smtSMART_OBJ_BASE, 0, Deleter<smtSMART_OBJ_BASE> > obj = deathObjs[oi];
        if (obj.pointee)
        {
            dsVECTOR<smtSMT_ENTER_INFO, 8> slots;
            memset(&slots, 0, 12);
            apCL cl;
            cl.file = kSmartCpp;
            cl.line = 307;
            slots.__cl = cl;
            obj.pointee->GetAvailableSlotsInfo(&slots, 1, nullptr);

            if (slots.nElem)
            {
                for (int i = 0; i < slots.nElem; ++i)
                {
                    smtSMT_ENTER_INFO &slot = slots[i];
                    if (!this->body.pointee->CanUseSmart(&obj, &slot.info))
                        continue;

                    const entTRACKER *trk = &slot.placeEnter->trk;
                    if (trk->type && trkHOLDER::IsValid(&trk->holder))
                    {
                        m3dV pos;
                        trk->GetPos(&pos);
                        float d2 = this->Dist_2(&pos);
                        if (d2 < bestDist2)
                        {
                            bestDist2 = d2;
                            best = obj;
                            bestSlot = i;
                        }
                    }
                    else
                    {
                        _apLog("~AI,Error~'%s':TryToUseAnimDeathSmt(): Can't use smt entry point '%s': tracker not set!",
                               this->names.nameInst,
                               slot.placeEnter->nameStartPoint.pBuffer->str);
                    }
                }
            }
            dlFree(slots.pData);
            slots.pData = nullptr;
        }
        obj.Reset();
    }

    if (!best.pointee || bestSlot < 0)
    {
        best.Reset();
        return false;
    }

    dsVECTOR<smtSMT_ENTER_INFO, 8> chosenSlots;
    memset(&chosenSlots, 0, 12);
    apCL cl2;
    cl2.file = kSmartCpp;
    cl2.line = 345;
    chosenSlots.__cl = cl2;
    best.pointee->GetAvailableSlotsInfo(&chosenSlots, 1, nullptr);

    dsPAIR<dsSTRID, dsSTRID> info = chosenSlots[bestSlot].info;
    bhpSMART_PAR par(&best, &info);

    if (!this->mind.best.pointee->SetBehaviorByParams(&par, "AnimDeathSmt"))
    {
        _apLog("~AI,Ssl,Error~'%s' Can't use anim death smart object - can't set SMART bhv",
               this->names.nameInst);
        // inlined ~bhpSMART_PAR
        par.smtObj.Reset();
        par.__vftable = (bhpPARAMS_vtbl *)&bhpPARAMS_vftable;
        dlFree(par.failNotifyAcceptorsSNum.pData);
        dlFree(chosenSlots.pData);
        best.Reset();
        return false;
    }

    this->st.pointee->die.val |= 0x2000u;

    aiBODY_IFACE *body = this->body.pointee;
    const dsSTRID *resetEvent = fsmFSM_EVENT::reset_();
    body->ActivateBodyCB(resetEvent, nullptr);

    // inlined ~bhpSMART_PAR
    par.smtObj.Reset();
    par.__vftable = (bhpPARAMS_vtbl *)&bhpPARAMS_vftable;
    dlFree(par.failNotifyAcceptorsSNum.pData);
    dlFree(chosenSlots.pData);
    best.Reset();
    return true;
}
