// aiBRAIN::implUseSmartObject @ 0x832B24C0
// ?implUseSmartObject@aiBRAIN@@UAA_NABV?$dsTSTRING@D@@00_N0@Z  (virtual)
//
// SSL "UseSmartObject(object, action, ...)" body: command the brain into a smart-object interaction.
// `object` names the smart object plus an optional "key/val" entry-point spec; `action` is the bhv
// tag; `anim` carries the invite mode; `instant` is the push-vs-set flag; `exitAction` is the path
// speed preset. When the brain is in scripted-teleport mode (mind bit 25) the call is forwarded to
// implUseSmartObjectInstant. Otherwise the smart object is looked up, an optional named entry point
// resolved, a bhpMAJOR_SMART_PAR built and set/pushed on the best-behaviour module. Returns false on
// any back-off/lookup/entry/set/push failure.
//
// DEVIATION: the decompiler packs `instant` (bool) and `exitAction` (dsTSTRING&) into one 64-bit
// operand and renames the params; the mangled signature (00_N0) is 3 strings, a bool, then a string,
// which is the declared (object, action, anim, instant, exitAction) order used here.
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiBRAIN_boundary.h"   // _apLog
#include "../../headers/ws/ai/aiMIND.h"
#include "../../headers/ws/ai/aiBEST_BASE.h"
#include "../../headers/ws/ai/aiSTATUS.h"
#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/ai/aiCON_CB_globals.h"   // aiPlanner
#include "../../headers/ws/smt/smtSMART_OBJ_BASE.h"
#include "../../headers/ws/smt/smtSMT_ENTER_INFO.h"
#include "../../headers/ws/bhp/bhpMAJOR_SMART_PAR.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/ds/dsPAIR.h"
#include "../../headers/ws/ds/dsSTRID.h"
#include "../../headers/ws/ds/dsVECTOR.h"
#include "../../headers/ws/ds/dsSHARED_PTR.h"
#include "../../headers/ws/ds/dsFLAGS.h"
#include "../../headers/apCL.h"

// dsSPrintf — format into the supplied dsTSTRING<char> out-param (returned by pointer). boundary.
extern dsTSTRING<char> *dsSPrintf(dsTSTRING<char> *out, const char *fmt, ...);
// SplitSmtObjName — split "obj[/key/val]" into object name + entry-point key/val out-params. boundary.
extern void SplitSmtObjName(const dsTSTRING<char> *src, dsTSTRING<char> *objName,
                            dsTSTRING<char> *entryKey, dsTSTRING<char> *entryVal);

// AI::SMT_INVITE_* — interned invite-mode ids (a8 globals). boundary.
namespace AI {
extern const dsSTRID SMT_INVITE_AI;
extern const dsSTRID SMT_INVITE_PLAYER;
extern const dsSTRID SMT_INVITE_AI_PLAYER;
}

bool aiBRAIN::implUseSmartObject(const dsTSTRING<char> &object, const dsTSTRING<char> &action,
                                 const dsTSTRING<char> &anim, bool instant,
                                 const dsTSTRING<char> &exitAction)
{
    dsTSTRING<char> callName;
    if (this->IsSSLCommonBackOff(
            *dsSPrintf(&callName, "UseSmartObject(\"%s\", \"%s\")",
                       object.pBuffer->str, action.pBuffer->str),
            dsFLAGS<SSL_COMMON_BACKOFF, int>{3}))
        return false;

    unsigned int mindFlags = (unsigned int)this->st.pointee->mind.val;

    // Scripted-teleport mode (bit 25): route through the instant variant.
    if ((mindFlags >> 25) & 1)
    {
        _apLog("~AI~'%s' ForceAF call UseSmartObject(\"%s\", \"%s\") via UseSmartObjectInstant()...",
               this->names.nameInst, object.pBuffer->str, action.pBuffer->str);
        return this->implUseSmartObjectInstant(object, action, instant);
    }

    dsTSTRING<char> objName(object);
    dsTSTRING<char> entryKey;
    dsTSTRING<char> entryVal;
    SplitSmtObjName(&object, &objName, &entryKey, &entryVal);

    dsSHARED_PTR<smtSMART_OBJ_BASE, 0, Deleter<smtSMART_OBJ_BASE> > smtObj =
        aiPlanner->FindSmartObject(objName);
    if (!smtObj.pointee)
    {
        _apLog("~AI,Ssl,Error~'%s' can't call UseSmartObject(\"%s\", \"%s\"): can't find smart object",
               this->names.nameInst, objName.pBuffer->str, action.pBuffer->str);
        return false;
    }

    // Resolve the named entry point (when a key/val spec was supplied). Left empty otherwise, letting
    // the planner pick.
    dsPAIR<dsSTRID, dsSTRID> entryInfo;
    if (entryKey.pBuffer->strLen || entryVal.pBuffer->strLen)
    {
        apCL cl = { "D:\\Projects\\code\\common\\src.sys\\gm_shared\\ai08\\a8_goal.cpp", 579 };
        dsVECTOR<smtSMT_ENTER_INFO, 8> slots(cl);
        smtObj.pointee->GetAvailableSlotsInfo(&slots, 1, nullptr);

        bool keyWildcard = (entryKey.pBuffer->strLen == 0);
        bool valWildcard = (entryVal.pBuffer->strLen == 0);
        for (int i = 0; i < slots.nElem; ++i)
        {
            bool keyMatch = keyWildcard ||
                            (dsTSTRING<char>(slots[i].info.key.CStr()) == entryKey);
            bool valMatch = valWildcard ||
                            (dsTSTRING<char>(slots[i].info.val.CStr()) == entryVal);
            if (keyMatch && valMatch)
            {
                entryInfo = slots[i].info;
                break;
            }
        }

        if (entryInfo.val.IsEmpty())
        {
            _apLog("~AI,Ssl,Error~'%s' can't call UseSmartObject(\"%s\", \"%s\"): can't find entry point or it is already locked",
                   this->names.nameInst, object.pBuffer->str, action.pBuffer->str);
            return false;
        }
    }

    bool haveEntry = !entryInfo.val.IsEmpty();
    bhpMAJOR_SMART_PAR par(&smtObj, haveEntry ? &entryInfo : nullptr);

    dsSTRID inviteId(anim, true);
    par.canInviteAI = (AI::SMT_INVITE_AI.id == inviteId.id ||
                       AI::SMT_INVITE_AI_PLAYER.id == inviteId.id);
    par.canInvitePlayer = (AI::SMT_INVITE_PLAYER.id == inviteId.id ||
                           AI::SMT_INVITE_AI_PLAYER.id == inviteId.id);
    par.pathSpeed = exitAction;
    if ((this->st.pointee->mind.val & 0x1000000) != 0)
        par.notifyTaskSys = true;

    aiBEST_BASE *best = this->mind.best.pointee;
    // DEVIATION: the best-module set/push argument tuple is not recoverable from the decompiler
    // (call shown with no operands); reconstructed from the sibling implUseSmartObjectInstant.
    if (instant)
    {
        if (!best->PushBehaviorByParams(&par, 0, nullptr))
        {
            _apLog("~AI,Ssl,Error~'%s' UseSmartObject(\"%s\", \"%s\"): can't push SMART bhv",
                   this->names.nameInst, objName.pBuffer->str, action.pBuffer->str);
            return false;
        }
    }
    else
    {
        if (!best->SetBehaviorByParams(&par, nullptr))
        {
            _apLog("~AI,Ssl,Error~'%s' UseSmartObject(\"%s\", \"%s\"): can't set SMART bhv",
                   this->names.nameInst, objName.pBuffer->str, action.pBuffer->str);
            return false;
        }
    }

    _apLog("~AI~'%s' call UseSmartObject(\"%s\", \"%s\")",
           this->names.nameInst, objName.pBuffer->str, action.pBuffer->str);
    return true;
}
