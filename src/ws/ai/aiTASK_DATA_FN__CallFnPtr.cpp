// aiTASK_DATA_FN::CallFnPtr(aiBRAIN*, aiTASK_BASE*) @ 0x832632C8
// ?CallFnPtr@aiTASK_DATA_FN@@IAA_NPAVaiBRAIN@@PAVaiTASK_BASE@@@Z
#include "../../headers/ws/ai/aiTASK_DATA_FN.h"
#include "../../headers/ws/ai/aiTASK_BASE.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiSTATUS.h"
#include "../../headers/ws/ds/dsFUNC_PTR_BASE.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsDATA_TYPE.h"
#include "../../headers/ws/ds/dsVECTOR.h"
#include "../../headers/ws/ds/dsSTRID.h"

extern void _apLog(const char *fmt, ...); // ?_apLog@@YAXPBDZZ — engine logger  boundary

// Mind-status bits toggled while a scripted task function runs (aiSTATUS::mind bitmask).
static const unsigned int AI_MIST_TRACK_BHV   = 0x1000000; // request behaviour-result tracking
static const unsigned int AI_MIST_FORCE_AF    = 0x2000000; // this call is a force-AF pass
static const unsigned int AI_MIST_CALL_CLEAR  = 0xFCFFFFFF; // clear both bits after the call

// Invoke the resolved scripted function, driving the force-after-fire / result-check state machine.
bool aiTASK_DATA_FN::CallFnPtr(aiBRAIN *pBrain, aiTASK_BASE *taskSys)
{
    bool wasReadyAF = (this->state == AI_TASK_DATA_FN_READY_AF);

    // Force-AF pass is due only when force-AF is enabled, we are in READY_AF, and the count is spent.
    bool doForceAF = this->afForceEnabled && this->state == AI_TASK_DATA_FN_READY_AF && this->afCount < 1;

    // We want to inspect the call/behaviour results when after-fire is active and a check is requested.
    bool wantResultCheck = (this->afForceEnabled || this->afCount > 0)
                        && (this->checkFnResult || this->checkBhvResults);
    bool trackBhv = wantResultCheck && this->checkBhvResults;

    if (doForceAF)
    {
        // Deviation: the raw __int128 forceAFfnChecker is a PPC pointer-to-member-function; call it.
        aiForceAFfnChecker chk = reinterpret_cast<aiForceAFfnChecker &>(this->forceAFfnChecker);
        if (!(pBrain->*chk)(this->funcParams))
        {
            this->AFRestart(pBrain);
            return false;
        }
    }

    dsDATA *args = nullptr;
    if (this->funcParams.nElem)
        args = &this->funcParams[0];

    dsDATA retVal;
    retVal.type = nullptr;

    // Publish the tracking flags into the brain status for the duration of the call.
    if (trackBhv) pBrain->st.pointee->mind.val |= AI_MIST_TRACK_BHV;
    else          pBrain->st.pointee->mind.val &= ~AI_MIST_TRACK_BHV;
    if (doForceAF) pBrain->st.pointee->mind.val |= AI_MIST_FORCE_AF;
    else           pBrain->st.pointee->mind.val &= ~AI_MIST_FORCE_AF;

    // Reset the owner's started-behaviour tally before the call so we can attribute new starts to it.
    taskSys->startedBhvList.nElem = 0;
    taskSys->bhvNotNeed = false;

    _apLog("~AITask~Call function $%s.%s()...", this->objName.pBuffer->str, this->fnName.CStr());

    bool ok = this->funcPtr->Call(args, this->funcParams.nElem, &retVal);

    pBrain->st.pointee->mind.val &= AI_MIST_CALL_CLEAR;

    if (!ok)
    {
        _apLog("~AITask,Error~Failed to call function $%s.%s()!!!",
               this->objName.pBuffer->str, this->fnName.CStr());
        this->state = AI_TASK_DATA_FN_DONE;
        if (retVal.type)
            retVal.type->Destroy(&retVal.storage);
        return false;
    }

    if (!wantResultCheck || doForceAF)
    {
        this->state = AI_TASK_DATA_FN_DONE;
    }
    else
    {
        if (wasReadyAF && this->afCount > 0)
            this->afCount = this->afCount - 1;

        if (trackBhv)
            this->afBhvList = taskSys->startedBhvList;

        bool fnResult = false;
        if (!this->checkFnResult
            || !retVal.GetValue<bool>(fnResult, DSD_CONV_RETRIEVE) // reads the call's bool result
            || fnResult)
        {
            if (trackBhv && this->afBhvList.nElem)
                this->state = AI_TASK_DATA_FN_WAIT_REPLY;
            else
                this->state = AI_TASK_DATA_FN_DONE;
        }
        else
        {
            this->AFRestart(pBrain);
        }
    }

    if (retVal.type)
        retVal.type->Destroy(&retVal.storage);
    return true;
}
