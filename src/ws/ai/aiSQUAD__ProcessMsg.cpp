#include "../../headers/ws/ai/aiSQUAD.h"
#include "../../headers/ws/msg/msgADDR.h"
#include "../../headers/ws/msg/msgRES.h"

// aiSQUAD::ProcessMsg(int, void*, msgADDR*) @ 0x832A9588
// ?ProcessMsg@aiSQUAD@@UAA?AW4msgRES@@HPAXPAVmsgADDR@@@Z
//
// Chain to the entity base handler. On the create message (msg == 1), if the squad's init-pending
// state bit (0x1) is still set, clear it and run OnCreate once. The base result is returned in
// every case.
msgRES aiSQUAD::ProcessMsg(int msg, void *pInfo, msgADDR *pSend)
{
    if (msg != 1)
        return entENTITY::ProcessMsg(msg, pInfo, pSend);

    msgRES result = entENTITY::ProcessMsg(1, pInfo, pSend);
    if (state.val & 1)
    {
        state.val &= ~1;
        OnCreate();
    }
    return result;
}
