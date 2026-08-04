#include "msgMSG.h"

// dlFree @ Saber debug heap free — boundary, not reversed here.
extern "C" void dlFree(void *ptr);

// msgMSG::DelInfo @ 0x82AC3770
// Release the message payload (if any) and reset the envelope to an empty/dead state.
void msgMSG::DelInfo()
{
    if (this->pInfo)
    {
        dlFree(this->pInfo);
        this->pInfo = nullptr;
    }
    this->pRecv = nullptr;
    this->pSend = nullptr;
    this->msg = -1;
    this->elapsedTime = 0.0f;
}
