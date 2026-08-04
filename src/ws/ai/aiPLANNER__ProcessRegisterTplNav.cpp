// aiPLANNER::ProcessRegisterTplNav @ 0x83246E70
// ?ProcessRegisterTplNav@aiPLANNER@@IAAXPAVmsgADDR@@@Z
//
// Register-message handler: when the render is enabled and the newly registered actor `pSend` is
// an entity (its runtime type derives from entDESC), queue it as a scene template with the nav
// manager so its waypoints get processed.
#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/nav/navMANAGER.h"
#include "../../headers/ws/msg/msgADDR.h"
#include "../../headers/ws/ds/dsTYPE_ID.h"
#include "../../headers/ws/ent/entENTITY.h"
#include "../../headers/ws/ap/apCOUNTER_TIME_CALL.h"
#include "../../headers/ws/ap/apCOUNTER_TIME_CALL_WRP.h"

extern int gsAppState;                       // gs app-state flag word (bit 0x2000 = render enabled)
struct entDESC { static dsTYPE_ID TYPE_ID; }; // ?TYPE_ID@entDESC@@2VdsTYPE_ID@@A (boundary static)

void aiPLANNER::ProcessRegisterTplNav(msgADDR *pSend)
{
    static apCOUNTER_TIME_CALL localTimeCounter("process_frame/ai/planner/ProcessRegisterTplNav");
    apCOUNTER_TIME_CALL_WRP<apCOUNTER_TIME_CALL> timerScope(&localTimeCounter);

    if ((gsAppState & 0x2000) != 0 && pSend) {
        dsTYPE_ID tid;
        if (pSend->__vftable->TypeID(pSend, &tid)->IsDerivedFrom(entDESC::TYPE_ID))
            navManager->AddSceneTplToQueue(reinterpret_cast<entENTITY *>(pSend));
    }
}
