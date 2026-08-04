// aiBRAIN::implUseSmartObjectInstant @ 0x83268E98
// ?implUseSmartObjectInstant@aiBRAIN@@UAA_NABV?$dsTSTRING@D@@0_N@Z  (virtual)
//
// Snap this brain instantly into a smart-object interaction. `nameSmtAndEntryPoint` is split into the
// object name plus an optional "key/val" entry-point spec. The smart object is looked up in the
// planner and its available entry-point slots gathered. When an entry-point spec is given the matching
// slot is chosen; otherwise the usable slot nearest the brain is chosen. A bhpSMART_PAR is built and
// either set (doPushBHV == false) or pushed as the best-behaviour module's behaviour, then the body's
// FSM reset callback fires. Returns false on any back-off/lookup/availability/set/push failure.
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiBRAIN_IFACE.h"        // Dist_2
#include "../../headers/ws/ai/aiBRAIN_boundary.h"      // _apLog
#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/ai/aiBODY_IFACE.h"
#include "../../headers/ws/ai/aiMIND.h"
#include "../../headers/ws/ai/aiBEST_BASE.h"
#include "../../headers/ws/ai/aiSTATUS.h"
#include "../../headers/ws/smt/smtSMART_OBJ_BASE.h"
#include "../../headers/ws/smt/smtSMT_ENTER_INFO.h"
#include "../../headers/ws/gm/gmPLACE_CHECKER.h"
#include "../../headers/ws/bhp/bhpSMART_PAR.h"
#include "../../headers/ws/ent/entTRACKER.h"
#include "../../headers/ws/ent/trkHOLDER.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/ds/dsPAIR.h"
#include "../../headers/ws/ds/dsSTRID.h"
#include "../../headers/ws/ds/dsVECTOR.h"
#include "../../headers/ws/ds/dsSHARED_PTR.h"
#include "../../headers/ws/ds/dsFLAGS.h"
#include "../../headers/ws/m3d/m3dV.h"
#include "../../headers/ws/m3d/m3d_boundary.h"         // m3dVZero
#include "../../headers/apCL.h"

// The ai08 planner singleton (declared locally to avoid aiEXIFACE.h's conflicting boundary defs).
extern aiPLANNER *aiPlanner;
// dsSPrintf — format into the supplied dsTSTRING<char> out-param (returned by pointer). boundary.
extern dsTSTRING<char> *dsSPrintf(dsTSTRING<char> *out, const char *fmt, ...);
// SplitSmtObjName — split "obj[/key/val]" into object name + entry-point key/val out-params. boundary.
extern void SplitSmtObjName(const dsTSTRING<char> *src, dsTSTRING<char> *objName,
                            dsTSTRING<char> *entryKey, dsTSTRING<char> *entryVal);

// boundary: the shared "smart reset" FSM event id (body FSM callback selector). Body external.
struct fsmFSM_EVENT { static const dsSTRID *reset_(); };
// boundary: gmPLACE_CHECKER (smtSMT_ENTER_INFO::placeEnter) — its ::trk (entTRACKER) and
// ::nameStartPoint (dsTSTRING) members are read here; the full checker layout is the next frontier.

bool aiBRAIN::implUseSmartObjectInstant(const dsTSTRING<char> &nameSmtAndEntryPoint,
                                        const dsTSTRING<char> &bhvName,
                                        bool doPushBHV)
{
    // Reject the call when the common SSL back-off gate is engaged (flags == 17).
    dsTSTRING<char> callName;
    if (this->IsSSLCommonBackOff(
            *dsSPrintf(&callName, "UseSmartObjectInstant(\"%s\", \"%s\")",
                       nameSmtAndEntryPoint.pBuffer->str, bhvName.pBuffer->str),
            dsFLAGS<SSL_COMMON_BACKOFF, int>{17}))
        return false;

    // Split "obj[/key/val]" into the object name and the optional entry-point key/val spec.
    dsTSTRING<char> objName(nameSmtAndEntryPoint);
    dsTSTRING<char> entryKey;
    dsTSTRING<char> entryVal;
    SplitSmtObjName(&nameSmtAndEntryPoint, &objName, &entryKey, &entryVal);

    dsSHARED_PTR<smtSMART_OBJ_BASE, 0, Deleter<smtSMART_OBJ_BASE> > smtObj =
        aiPlanner->FindSmartObject(objName);
    if (!smtObj.pointee)
    {
        _apLog("~AI,Ssl,Error~'%s' can't call UseSmartObjectInstant(\"%s\", \"%s\"): can't find smart object",
               this->names.nameInst, objName.pBuffer->str, bhvName.pBuffer->str);
        return false;
    }

    apCL cl = { "D:\\Projects\\code\\common\\src.sys\\gm_shared\\ai08\\b8_smart.cpp", 66 };
    dsVECTOR<smtSMT_ENTER_INFO, 8> slots(cl);
    smtObj.pointee->GetAvailableSlotsInfo(&slots, 1, nullptr);

    if (!slots.nElem)
    {
        _apLog("~AI,Error~'%s' UseSmartObjectInstant(): all entry points of Smart Object '%s' are locked",
               this->names.nameInst, smtObj.pointee->name.CStr());
        return false;
    }

    int selected = -1;
    if (entryKey.pBuffer->strLen || entryVal.pBuffer->strLen)
    {
        // Named entry point: an empty key/val component acts as a wildcard on that side.
        bool keyWildcard = (entryKey.pBuffer->strLen == 0);
        bool valWildcard = (entryVal.pBuffer->strLen == 0);
        for (int i = 0; i < slots.nElem; ++i)
        {
            bool valMatch = valWildcard ||
                            (dsTSTRING<char>(slots[i].info.val.CStr()) == entryVal);
            bool keyMatch = keyWildcard ||
                            (dsTSTRING<char>(slots[i].info.key.CStr()) == entryKey);
            if (valMatch && keyMatch)
            {
                selected = i;
                break;
            }
        }
        if (selected < 0)
        {
            _apLog("~AI,Ssl,Error~'%s' can't call UseSmartObjectInstant(\"%s\", \"%s\"): can't find entry point or it is already locked",
                   this->names.nameInst, nameSmtAndEntryPoint.pBuffer->str, bhvName.pBuffer->str);
            return false;
        }
    }
    else
    {
        // No entry-point spec: pick the usable slot whose tracker is nearest the brain.
        double minDist = 2000000.0;
        for (int i = 0; i < slots.nElem; ++i)
        {
            if (!this->body.pointee->CanUseSmart(&smtObj, &slots[i].info))
                continue;

            const entTRACKER &trk = slots[i].placeEnter->trk; // gmPLACE_CHECKER::trk — boundary
            if (trk.type && trkHOLDER::IsValid(&slots[i].placeEnter->trk.holder))
            {
                m3dV pos = m3dVZero;
                slots[i].placeEnter->trk.GetPos(&pos);
                double dist = this->aiBRAIN_IFACE::Dist_2(&pos);
                if (dist < minDist)
                {
                    minDist = dist;
                    selected = i;
                }
            }
            else
            {
                _apLog("~AI,Error~'%s':UseSmartObjectInstant(): Can't use smt entry point '%s': tracker not set!",
                       this->names.nameInst, slots[i].placeEnter->nameStartPoint.pBuffer->str);
            }
        }
        if (selected < 0)
        {
            _apLog("~AI,Error~'%s' UseSmartObjectInstant(): all entry points of Smart Object '%s' are unavailable",
                   this->names.nameInst, smtObj.pointee->name.CStr());
            return false;
        }
    }

    unsigned int mindFlags = (unsigned int)this->st.pointee->mind.val;

    // ForceAF: when the "scripted teleport" mind flag (bit 25) is set, snap the brain to the slot.
    if ((mindFlags >> 25) & 1)
    {
        m3dV pos;
        slots[selected].placeEnter->trk.GetPos(&pos);
        this->Transport(&pos);
        _apLog("~AI~'%s' ForceAF call UseSmartObjectInstant(\"%s\", \"%s\")",
               this->names.nameInst, objName.pBuffer->str, bhvName.pBuffer->str);
    }

    dsPAIR<dsSTRID, dsSTRID> entryInfo = slots[selected].info;
    bhpSMART_PAR params(&smtObj, &entryInfo);
    if ((mindFlags & 0x1000000) != 0 && ((mindFlags >> 25) & 1) == 0)
        params.notifyTaskSys = true;

    aiBEST_BASE *best = this->mind.best.pointee;
    // DEVIATION: the aiBEST_BASE boundary decls model Set/PushBehaviorByParams' first param as
    // bhpSNS_PAR*/bhpPARAMS*; the true virtuals take bhpPARAMS* and (per the disassembly)
    // SetBehaviorByParams returns a bool here.
    if (!doPushBHV)
    {
        if (!best->SetBehaviorByParams(&params, nullptr))
        {
            _apLog("~AI,Ssl,Error~'%s' UseSmartObjectInstant(\"%s\", \"%s\"): can't set SMART bhv",
                   this->names.nameInst, objName.pBuffer->str, bhvName.pBuffer->str);
            return false;
        }
    }
    else
    {
        if (!best->PushBehaviorByParams(&params, 0, nullptr))
        {
            _apLog("~AI,Ssl,Error~'%s' UseSmartObjectInstant(\"%s\", \"%s\"): can't push SMART bhv",
                   this->names.nameInst, objName.pBuffer->str, bhvName.pBuffer->str);
            return false;
        }
    }

    aiBODY_IFACE *body = this->body.pointee;
    body->ActivateBodyCB(fsmFSM_EVENT::reset_(), nullptr);
    _apLog("~AI~'%s' call UseSmartObjectInstant(\"%s\", \"%s\")",
           this->names.nameInst, objName.pBuffer->str, bhvName.pBuffer->str);
    return true;
    // params / slots / smtObj / the dsTSTRING locals release their storage at scope end.
}
