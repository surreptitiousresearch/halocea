#include "../../headers/ws/ai/aiISA.h"
#include "../../headers/ws/ai/aiBRAIN.h"

// aiISA::NotifyPhysObject(const m3dV&) @ 0x832A12D0
// ?NotifyPhysObject@aiISA@@UAAXABUm3dV@@@Z (virtual)
// When phys-object reports are enabled and the object is within 4m (dist^2 <= 16), raise an
// immediate team PHYS_OBJ alert at `posObject`.
void aiISA::NotifyPhysObject(const m3dV &posObject)
{
    if ((this->state.val & AI_ISAST_REPORT_PHYS_OBJ)
        && this->pBrain->Dist_2(&posObject) <= 16.0f)
    {
        aiISA::ALERT_PARAMS params = {
            AI_ALERT_PHYS_OBJ,
            posObject,
            dsTSTRING<char>("PHYS_OBJ"),
            true,  // isAlertTeam
            false, // isUseDelay
        };
        this->Alert(params);
    }
}
