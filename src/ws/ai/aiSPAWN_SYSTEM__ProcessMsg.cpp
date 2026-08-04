#include "../../headers/ws/ai/aiSPAWN_SYSTEM.h"
#include "../../headers/ws/msg/msgADDR.h"

// aiSPAWN_SYSTEM::ProcessMsg(int, void*, msgADDR*) @ 0x832A1A98
// ?ProcessMsg@aiSPAWN_SYSTEM@@MAA?AW4msgRES@@HPAXPAVmsgADDR@@@Z (protected virtual)
// Intercept the spawn-point invalidate (1013), load (1015) and save (1016) domain messages, then
// defer to the base domain-system handler.
msgRES aiSPAWN_SYSTEM::ProcessMsg(int msg, void *pInfo, msgADDR *pSend)
{
    switch (msg)
    {
    case 1013:
        this->isPointsLoaded = 0;
        break;
    case 1015:
        this->LoadDomainsInfo();
        break;
    case 1016:
        this->DestroyTempDomainInfo();
        this->SaveDomainsInfo();
        break;
    }

    return gsDOMAIN_SYS_ProcessMsg(this, msg, pInfo, pSend);
}
