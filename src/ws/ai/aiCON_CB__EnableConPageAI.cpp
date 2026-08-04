#include "../../headers/ws/ai/aiCON_CB.h"
#include "../../headers/ws/ai/aiCON_CB_globals.h"
#include "../../headers/ws/fio/fioFILE.h"

// aiCON_CB::EnableConPageAI @ 0x831827D0
// Toggle the remote AI console page on/off, reset the main/extra line selectors, and push a
// full refresh of every sub-list.
void aiCON_CB::EnableConPageAI(bool enable)
{
    fioFILE *msg = &apConsole->StartMsg(this->pageName.pBuffer->str,
                                        this->msgCtrlName.pBuffer->str,
                                        sn_EnablePage_1.pBuffer->str);
    int flag = enable;
    msg->WriteData(&flag, 4, 4);
    if (this->pageVersionValid)
        apConsole->SendMsg(*msg);
    else
        apConsole->CancelMsg(*msg);

    this->SendSelectorMain(25);
    this->SendSelectorExtra(0);
    this->SendAll();
}
