#include "msgMSG.h"
#include "msgADDR.h"
#include <string.h> // memcpy

// dlMalloc @ Saber debug heap alloc (size, file, line) — boundary, not reversed here.
extern "C" void *dlMalloc(unsigned int size, const char *file, unsigned int line);

// msgMSG::SetInfo @ 0x82AC36A0
// Populate the envelope and deep-copy `sizeInfo` bytes of payload into a freshly allocated
// buffer. Returns 1 on success, 0 if the payload allocation failed.
// NOTE: the decompiler typed `elapsedTime` as double; the M mangle token and the float store at
// 0x82AC36F4 confirm it is a float (PPC float-slot ABI artifact).
int msgMSG::SetInfo(msgADDR *pRecv, int msg, int sizeInfo, void *pInfo, msgADDR *pSend,
                    float elapsedTime)
{
    this->pRecv = pRecv;
    this->pSend = pSend;
    this->msg = msg;
    this->elapsedTime = elapsedTime;
    this->sizeInfo = sizeInfo;

    if (pInfo)
    {
        this->pInfo = dlMalloc((unsigned int)sizeInfo,
                               "D:\\Projects\\code\\common\\src.sys\\gm_sys\\msg\\msg_queue.cpp",
                               0x24u);
        if (!this->pInfo)
            return 0;
        memcpy(this->pInfo, pInfo, (unsigned int)sizeInfo);
    }
    return 1;
}
