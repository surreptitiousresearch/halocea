// aiBRAIN::implUseSmartIdleInstant @ 0x8327EBA8
// ?implUseSmartIdleInstant@aiBRAIN@@UAA_NABV?$dsTSTRING@D@@0@Z  (virtual)
//
// Snap this brain instantly into a smart-object idle animation. Looks up the smart object in the
// planner, gathers its available entry-point slots, and picks the usable slot whose entry tracker is
// closest to the brain. Builds a bhpIDLE_SMART_PAR and sets it as the best-behaviour module's current
// behaviour. Returns false when the SSL common back-off gate rejects the call, when the object/slot
// can't be found or is locked/unavailable, or when the behaviour can't be set; true on success.
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiBRAIN_IFACE.h"       // Dist_2
#include "../../headers/ws/ai/aiBRAIN_boundary.h"     // _apLog
#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/ai/aiBODY_IFACE.h"
#include "../../headers/ws/ai/aiMIND.h"
#include "../../headers/ws/ai/aiBEST_BASE.h"
#include "../../headers/ws/ai/aiWATCHER_BRAIN_boundaries.h" // aiBID / AI_BID_IDLE_SMART
#include "../../headers/ws/smt/smtSMART_OBJ_BASE.h"
#include "../../headers/ws/smt/smtSMT_ENTER_INFO.h"
#include "../../headers/ws/gm/gmPLACE_CHECKER.h"
#include "../../headers/ws/bhp/bhpIDLE_SMART_PAR.h"
#include "../../headers/ws/ent/entTRACKER.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/ds/dsPAIR.h"
#include "../../headers/ws/ds/dsSTRID.h"
#include "../../headers/ws/ds/dsVECTOR.h"
#include "../../headers/ws/ds/dsSHARED_PTR.h"
#include "../../headers/ws/ds/dsFLAGS.h"
#include "../../headers/ws/m3d/m3dV.h"
#include "../../headers/ws/m3d/m3d_boundary.h"        // m3dVZero
#include "../../headers/apCL.h"

// The ai08 planner singleton (declared locally to avoid aiEXIFACE.h's conflicting boundary defs).
extern aiPLANNER *aiPlanner;
// dsSPrintf — format into the supplied dsTSTRING<char> out-param (returned by pointer). boundary.
extern dsTSTRING<char> *dsSPrintf(dsTSTRING<char> *out, const char *fmt, ...);

// boundary: gmPLACE_CHECKER (smtSMT_ENTER_INFO::placeEnter) — its ::trk (entTRACKER) member is read
// here for the entry-point tracker; the full checker layout is the next frontier.

bool aiBRAIN::implUseSmartIdleInstant(const dsTSTRING<char> &smtObjName,
                                      const dsTSTRING<char> &bhvName)
{
    // Reject the call when the common SSL back-off gate is engaged (flags == 1).
    dsTSTRING<char> callName;
    if (this->IsSSLCommonBackOff(
            *dsSPrintf(&callName, "UseSmartIdleInstant(\"%s\", \"%s\")",
                       smtObjName.pBuffer->str, bhvName.pBuffer->str),
            dsFLAGS<SSL_COMMON_BACKOFF, int>{1}))
        return false;

    dsSHARED_PTR<smtSMART_OBJ_BASE, 0, Deleter<smtSMART_OBJ_BASE> > smtObj =
        aiPlanner->FindSmartObject(smtObjName);
    if (!smtObj.pointee)
    {
        _apLog("~AI,Ssl,Error~'%s' can't call UseSmartIdleInstant(\"%s\", \"%s\"): can't find smart object",
               this->names.nameInst, smtObjName.pBuffer->str, bhvName.pBuffer->str);
        return false;
    }

    apCL cl = { "D:\\Projects\\code\\common\\src.sys\\gm_shared\\ai08\\b8_idle.cpp", 49 };
    dsVECTOR<smtSMT_ENTER_INFO, 8> slots(cl);
    smtObj.pointee->GetAvailableSlotsInfo(&slots, 1, nullptr);

    if (!slots.nElem)
    {
        _apLog("~AI,Error~'%s' UseSmartIdleInstant(): all entry points of Smart Object '%s' are locked",
               this->names.nameInst, smtObj.pointee->name.CStr());
        return false;
    }

    // Pick the usable entry-point slot whose tracker is nearest to the brain.
    int best = -1;
    double minDist = 2000000.0;
    for (int i = 0; i < slots.nElem; ++i)
    {
        if (this->body.pointee->CanUseSmart(&smtObj, &slots[i].info))
        {
            m3dV pos = m3dVZero;
            slots[i].placeEnter->trk.GetPos(&pos); // gmPLACE_CHECKER::trk — boundary
            double dist = this->aiBRAIN_IFACE::Dist_2(&pos);
            if (dist < minDist)
            {
                minDist = dist;
                best = i;
            }
        }
    }

    if (best < 0)
    {
        _apLog("~AI,Error~'%s' UseSmartIdleInstant(): all entry points of Smart Object '%s' are unavailable",
               this->names.nameInst, smtObj.pointee->name.CStr());
        return false;
    }

    // Build the idle-smart behaviour parameters and set them as the current best behaviour.
    dsPAIR<dsSTRID, dsSTRID> entryInfo = slots[best].info;
    bhpIDLE_SMART_PAR params;
    params.typeBehav = AI_BID_IDLE_SMART;
    params.smtObj = smtObj;                    // dsSHARED_PTR::operator=
    params.timeToExecute = 0.0f;
    params.entryPointInfo.val = entryInfo.val; // decompiler carries only info.val (entry-point action id)

    const char *name = bhvName.pBuffer->strLen ? bhvName.pBuffer->str : nullptr;
    // DEVIATION: the aiBEST_BASE boundary decl models SetBehaviorByParams as (bhpSNS_PAR*, const char*)
    // returning void; the true virtual takes a bhpPARAMS* and (per the disassembly) returns a bool.
    bool ok = this->mind.best.pointee->SetBehaviorByParams(&params, name);
    if (ok)
    {
        _apLog("~AI,Ssl~'%s' call UseSmartIdleInstant(\"%s\", \"%s\")",
               this->names.nameInst, smtObjName.pBuffer->str, bhvName.pBuffer->str);
        return true;
    }

    _apLog("~AI,Ssl,Error~'%s' UseSmartIdleInstant(\"%s\", \"%s\"): can't set SMART bhv",
           this->names.nameInst, smtObjName.pBuffer->str, bhvName.pBuffer->str);
    return false;
    // params / slots / smtObj release their storage at scope end.
}
