/* ?Update@sslDELAYED_FUNCTION@@QAAHM@Z @0x82AF0F90 */
#include "sslDELAYED_FUNCTION.h"
#include "sslOBJ_REF.h"
#include "sslOBJECT.h"
#include "sslCLASS_REF.h"
#include "sslSYSTEM.h"
#include "sslERROR.h"
#include "../ds/dsDATA.h"
#include "../ds/dsSTATE_MGR.h"

// 0x82AF0F90 — sslDELAYED_FUNCTION::Update. Returns int (no sret). `time` is declared float but
// arrives widened to double in the decompile (float-slot ABI); the body uses the float value.
// Tick the pending call and, once its timer has expired, fire it (restoring the saved sync context
// around the call) and clear the record.
int sslDELAYED_FUNCTION::Update(float time)
{
    sslOBJECT *pObject = this->self.pObject;
    if (!pObject)
        return 1;

    if ((pObject->state.state & 1) != 0 || this->funcIdx == -1)
        return 1; // object still constructing, or the record is empty: keep it

    if (((pObject->state.state >> 1) & 1) != 0)
        return 0; // object flagged (being torn down): suppress this tick
    if (this->pauseLocks)
        return 0; // paused

    this->timeLeft -= time;
    if (this->timeLeft > 0.0f)
        return 0; // not due yet

    sslCLASS_REF objClass = this->self.GetClass();
    sslSYSTEM *ssl = objClass.GetSslSystem();

    // Save the current "SyncContext" runtime state, install this call's context, restore afterwards.
    static int syncContextStateId = gStateMgr->RegisterState("SyncContext", false);
    dsDATA savedContext;
    savedContext.type = nullptr;
    savedContext.StoreValue(*gStateMgr->GetState(syncContextStateId));
    gStateMgr->SetState(syncContextStateId, &this->syncContext);

    sslOBJ_REF callerCopy(this->caller);
    sslERROR err = this->self.CallFunc(this->funcIdx, this->params.nElem, this->params.pData,
                                       nullptr, callerCopy);
    (void)err;

    if (savedContext.type)
        gStateMgr->SetState(syncContextStateId, &savedContext);
    else
        gStateMgr->EraseState(syncContextStateId);

    this->Clear();
    ssl->isDelayedFuncCall = false;
    return 1;
}
