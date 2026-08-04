#include "dsERASE_MANAGER.h"
#include "dsLF_SORTED_MAP.h"
#include "dsSTRID.h"

struct dsSTR_HASH; // boundary — string hash policy
struct dsSTR_CMP;  // boundary — string compare policy

typedef dsLF_SORTED_MAP<const char *, dsSTRID, dsSTR_HASH, dsSTR_CMP>::LINE_ERASER StrStridLineEraser;

// dsERASE_MANAGER<dsLF_SORTED_MAP<char const*,dsSTRID,dsSTR_HASH,dsSTR_CMP>::LINE_ERASER>::ctor
// @ 0x82529EB0 — zero the deferred-erase queue and bring up a fresh access lock.
template<>
dsERASE_MANAGER<StrStridLineEraser>::dsERASE_MANAGER()
{
    apCL callSite;
    callSite.line = 23;
    callSite.file = "Unknown";

    this->threadAllocMask = 0;
    this->eraseQueue.data.pData = nullptr;
    this->eraseQueue.data.nElem = 0;
    this->eraseQueue.data.allocated = 0;
    this->eraseQueue.head = 0;
    this->eraseQueue.tail = 0;
    this->eraseQueue.length = 0;
    this->eraseQueue.data.__cl = callSite;

    this->accessLock.isInited = false;
    this->accessLock.threadNmb = -1;
    this->accessLock.locker_file = nullptr;
    this->accessLock.locker_line = -1;
    this->accessLock.name = "Unknown";
    this->accessLock.Init();
}
