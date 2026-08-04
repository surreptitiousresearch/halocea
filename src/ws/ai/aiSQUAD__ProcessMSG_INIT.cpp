#include "../../headers/ws/ai/aiSQUAD.h"
#include "../../headers/ws/msg/msgADDR.h"
#include "../../headers/ws/msg/msgRES.h"

// aiSQUAD::ProcessMSG_INIT(int, void*, msgADDR*) @ 0x832A8EE8
// ?ProcessMSG_INIT@aiSQUAD@@AAA?AW4msgRES@@HPAXPAVmsgADDR@@@Z  (private)
// Dispatch the message through the base entity, then — on the init message (state bit 0x1) — clear
// the init-pending flag and fire the OnCreate SSL event. Returns the base dispatch result.
msgRES aiSQUAD::ProcessMSG_INIT(int msg, void *pInfo, msgADDR *pSend)
{
    msgRES res = entENTITY::ProcessMsg(msg, pInfo, pSend);

    if ((state.val & 1) != 0)
    {
        state.val &= ~1;
        OnCreate();
    }
    return res;
}
