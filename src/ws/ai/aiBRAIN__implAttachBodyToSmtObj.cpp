// aiBRAIN::implAttachBodyToSmtObj @ 0x83267A00
// ?implAttachBodyToSmtObj@aiBRAIN@@UAA_NABV?$dsTSTRING@D@@0@Z  (virtual)
//
// Attach this brain's body to the anchor of a named smart object. Looks the smart object up in the
// planner, gathers its currently-available entry-point slots, picks either the slot whose entry-point
// name matches `entryPointName` (or the first slot when the name is empty), and drives the body's
// AttachToSmart() virtual. Returns true when the brain is termed (nothing to do) or the attach
// succeeds; false on any lookup/availability/attach failure.
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiBRAIN_boundary.h"   // _apLog
#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/ai/aiBODY_IFACE.h"
#include "../../headers/ws/smt/smtSMART_OBJ_BASE.h"
#include "../../headers/ws/smt/smtSMT_ENTER_INFO.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/ds/dsPAIR.h"
#include "../../headers/ws/ds/dsSTRID.h"
#include "../../headers/ws/ds/dsVECTOR.h"
#include "../../headers/ws/ds/dsSHARED_PTR.h"
#include "../../headers/apCL.h"

// The ai08 planner singleton (declared locally to avoid aiEXIFACE.h's conflicting boundary defs).
extern aiPLANNER *aiPlanner;

bool aiBRAIN::implAttachBodyToSmtObj(const dsTSTRING<char> &smtObjName,
                                     const dsTSTRING<char> &entryPointName)
{
    // stateAddr bit 0x2 set == the actor is termed / not accepting messages.
    if (((this->stateAddr.state >> 1) & 1) != 0)
    {
        _apLog("~AI,Ssl,Error~'%s' can't call AttachBodyToSmtObj(\"%s\", \"%s\"): AI is termed",
               this->names.nameInst, smtObjName.pBuffer->str, entryPointName.pBuffer->str);
        return true;
    }

    dsSHARED_PTR<smtSMART_OBJ_BASE, 0, Deleter<smtSMART_OBJ_BASE> > smtObj =
        aiPlanner->FindSmartObject(smtObjName);
    if (!smtObj.pointee)
    {
        _apLog("~AI,Ssl,Error~'%s' can't call AttachBodyToSmtObj(\"%s\", \"%s\"): can't find smart object",
               this->names.nameInst, smtObjName.pBuffer->str, entryPointName.pBuffer->str);
        return false;
    }

    // Gather the currently-available entry-point slots.
    apCL cl = { "D:\\Projects\\code\\common\\src.sys\\gm_shared\\ai08\\b8_smart.cpp", 235 };
    dsVECTOR<smtSMT_ENTER_INFO, 8> slots(cl);
    smtObj.pointee->GetAvailableSlotsInfo(&slots, 1, nullptr);

    if (!slots.nElem)
    {
        _apLog("~AI,Error~'%s' AttachBodyToSmtObj(): all entry points of Smart Object '%s' are locked",
               this->names.nameInst, smtObj.pointee->name.CStr());
        return false;
    }

    // Select the entry-point slot.
    int selected = 0;
    if (entryPointName.pBuffer->strLen)
    {
        int found = -1;
        for (int i = 0; i < slots.nElem; ++i)
        {
            // slot.info.val is the entry-point name id; compared against the requested name.
            // dsSTRID compares against a C string (operator==(const char*)).
            if (slots[i].info.val == entryPointName.CStr())
            {
                found = i;
                break;
            }
        }
        if (found < 0)
        {
            _apLog("~AI,Error~'%s' AttachBodyToSmtObj(): entry point '%s' of Smart Object '%s' not found or unavailable",
                   this->names.nameInst, entryPointName.pBuffer->str,
                   smtObj.pointee->name.CStr());
            return false;
        }
        selected = found;
    }

    dsPAIR<dsSTRID, dsSTRID> entryInfo = slots[selected].info;
    aiBODY_IFACE *body = this->body.pointee;
    // DEVIATION: decompiler reads the AttachToSmart return as an 8-bit bool; boundary decl returns bool.
    bool attached = body->AttachToSmart(&smtObj, &entryInfo, 0);
    if (!attached)
    {
        _apLog("~AI,Error~'%s' AttachBodyToSmtObj(): body->AttachToSmart() failed",
               this->names.nameInst);
    }
    return attached;
    // slots (dsVECTOR) and smtObj (dsSHARED_PTR) release their storage at scope end.
}
