#include "../../headers/ws/ai/aiBODY.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/prop/propBODY_FSM.h"
#include "../../headers/ws/mdl/mdlCMD2BODY.h"
#include "../../headers/ws/mdl/mdlCDT_SETTINGS.h"

// 0x832ABD38 ?DbgGetRadCdt@aiBODY@@UBAMXZ
// The body's CDT collision radius: from the body FSM's per-frame command block when present, else
// from the owning brain's CDT tuning params.
float aiBODY::DbgGetRadCdt() const
{
    propBODY_FSM *bodyFsm = reinterpret_cast<propBODY_FSM *>(this->bodyFsm);
    if (bodyFsm)
        return bodyFsm->Cmd2Body().cdtParams.cdtSettings.radCdt;

    return this->pBrain->GetCdtParams()->radCdt;
}
