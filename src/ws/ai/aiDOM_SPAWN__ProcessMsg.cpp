#include "../../headers/ws/ai/aiDOM_SPAWN.h"
#include "../../headers/ws/ai/aiDOM_SPAWN_boundary.h"
#include "../../headers/ws/msg/msgADDR.h"

struct aiBRAIN_IFACE; // messaging peer (AccountDead arg)  boundary

// aiDOM_SPAWN::ProcessMsg @ 0x832656B8
// ?ProcessMsg@aiDOM_SPAWN@@MAA?AW4msgRES@@HPAXPAVmsgADDR@@@Z (virtual)
// Intercept the debug-render (1001) and unit-death (1261) messages, then defer to gsDOMAIN.
msgRES aiDOM_SPAWN::ProcessMsg(int msg, void *pInfo, msgADDR *pSend)
{
    if (msg == 1001)
        this->DebugRender(1001, (unsigned int)pInfo);
    else if (msg == 1261)
        this->AccountDead((aiBRAIN_IFACE *)pSend);

    return gsDOMAIN_ProcessMsg(this, msg, pInfo, pSend);
}
