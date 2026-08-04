// aiPLANNER::ProcessMsg @ 0x83247330
// ?ProcessMsg@aiPLANNER@@UAA?AW4msgRES@@HPAXPAVmsgADDR@@@Z
//
// Engine-message handler. Routes the AI-relevant messages to the planner's subsystems, always
// forwards the message to the nav editor, and chains to the base iaIACTOR::ProcessMsg for its
// return value.
//   1025/1026 — rigid-body start-move / start-rest, to the SMR subsystem
//   1022       — entity sound notice
//   3          — actor registered: queue nav template + off-AI debug hook
//   4          — actor unregistered: disconnect nav template + unblock removed instance
//   1001       — render debug info
#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/ai/aiPHYS_SMR.h"
#include "../../headers/ws/msg/msgADDR.h"
#include "../../headers/ws/ned/nedNAV_EDITOR_boundary.h"
#include "../../headers/ws/ia/iaIACTOR.h"

struct entSND_NOTICE; // ent — sound-notice payload (pointer only)
struct gsREND_DATA;   // gs  — per-frame render data (pointer only)

msgRES aiPLANNER::ProcessMsg(int msg, void *pInfo, msgADDR *pSend)
{
    if (msg > 1022) {
        if (msg == 1025)
            (&smr)->ProcessRbStartMove(static_cast<physRB_BASE *>(pInfo));
        else if (msg == 1026)
            (&smr)->ProcessRbStartRest(static_cast<physRB_BASE *>(pInfo));
    } else {
        switch (msg) {
            case 1022:
                ProcessSound(static_cast<const entSND_NOTICE *>(pInfo));
                break;
            case 3:
                ProcessRegisterTplNav(pSend);
                ProcessOffAI(pSend);
                break;
            case 4:
                ProcessUnRegisterTplNav(static_cast<msgADDR *>(pInfo));
                ProcessUnblockNavByRemovedInst(pSend);
                break;
            case 1001:
                RenderDebugInfo(static_cast<gsREND_DATA *>(pInfo));
                break;
        }
    }

    navEditor->ProcessMsg(msg);
    return reinterpret_cast<iaIACTOR *>(this)->ProcessMsg(msg, pInfo, pSend);
}
