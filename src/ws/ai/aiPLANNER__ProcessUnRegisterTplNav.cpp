// aiPLANNER::ProcessUnRegisterTplNav @ 0x83246F40
// ?ProcessUnRegisterTplNav@aiPLANNER@@IAAXPAVmsgADDR@@@Z
//
// Unregister-message handler: when the render is enabled and the removed actor `pSend` is an
// entity that owns an animated instance, disconnect that instance's waypoints from the nav manager.
#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/nav/navMANAGER.h"
#include "../../headers/ws/msg/msgADDR.h"
#include "../../headers/ws/ds/dsTYPE_ID.h"
#include "../../headers/ws/ent/entENTITY.h"
#include "../../headers/ws/ap/apCOUNTER_TIME_CALL.h"
#include "../../headers/ws/ap/apCOUNTER_TIME_CALL_WRP.h"

extern int gsAppState;                       // gs app-state flag word (bit 0x2000 = render enabled)
struct entDESC { static dsTYPE_ID TYPE_ID; }; // ?TYPE_ID@entDESC@@2VdsTYPE_ID@@A (boundary static)

void aiPLANNER::ProcessUnRegisterTplNav(msgADDR *pSend)
{
    static apCOUNTER_TIME_CALL localTimeCounter("process_frame/ai/planner/ProcessUnRegisterTplNav");
    apCOUNTER_TIME_CALL_WRP<apCOUNTER_TIME_CALL> timerScope(&localTimeCounter);

    if ((gsAppState & 0x2000) != 0 && pSend) {
        dsTYPE_ID tid;
        if (pSend->__vftable->TypeID(pSend, &tid)->IsDerivedFrom(entDESC::TYPE_ID)) {
            animINST *pInst = reinterpret_cast<entENTITY *>(pSend)->pInst;
            if (pInst)
                navManager->DisconnectInstWPs(pInst);
        }
    }
}
