#include "msgTIME_QUEUE.h"

// msgTIME_QUEUE::FreeMsg @ 0x82AC3A68
// Tear down a dequeued delayed message: release its payload, delete the object, null the slot.
void msgTIME_QUEUE::FreeMsg(msgMSG_TIME **ppMsg)
{
    (*ppMsg)->DelInfo();      // msgMSG_TIME derives from msgMSG
    ::operator delete(*ppMsg);
    *ppMsg = nullptr;
}
