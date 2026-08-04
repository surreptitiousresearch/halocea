#include "MSG_PROCESSOR_BASE.h"
#include "../ds/ds_assert_boundary.h"
#include "../ap/apProfileTimer.h"

// os primitives -- boundary (inline extern, matching sibling ws/* convention).
extern "C" unsigned __int64 osGetPerfCounter(void);      // boundary
extern "C" unsigned __int64 osGetPerfFrequancy(void);    // boundary
extern "C" unsigned int     osGetTime(void);              // boundary
extern "C" void             osSleep(int ms);               // boundary
void _apLog(const char *format, ...);                       // boundary

namespace con {

// Shared by both discard-notification sites below (id==2 purge on entry, and stale id==2
// entries skipped over while scanning for a response). Not a separate DB symbol -- the
// decompiler inlines this identical sequence at both call sites; factored out here purely
// for readability. Sends an empty-payload message with header {len=0, id=0x4000000}
// (little-endian, matching SendMsg's wire byte order -- see MSG_PROCESSOR_BASE__SendMsg.cpp).
static void SendDiscardNotification(MSG_PROCESSOR_BASE *self)
{
    osGetPerfCounter();
    if (!self->isConnected)
        return;

    self->lock.Lock(nullptr, 0);
    int outBufLen = self->outBuf.nElem;
    self->lock.Unlock(nullptr, 0);
    while (outBufLen >= 0x100000) {
        osSleep(1);
        self->lock.Lock(nullptr, 0);
        outBufLen = self->outBuf.nElem;
        self->lock.Unlock(nullptr, 0);
    }

    unsigned char lenHeader[4]  = { 0, 0, 0, 0 };
    unsigned char idHeader[4]   = { 0, 0, 0, 4 }; // little-endian bytes of 0x4000000

    self->lock.Lock(nullptr, 0);
    self->outBuf.Insert(self->outBuf.nElem, lenHeader, 4);
    self->outBuf.Insert(self->outBuf.nElem, idHeader, 4);
    self->outBuf.Insert(self->outBuf.nElem, nullptr, 0);
    osGetPerfCounter();
    osGetPerfFrequancy();
    self->lock.Unlock(nullptr, 0);
}

// 0x8309B718 -- send `data` as an id==2 request and block (polling, with exponential-backoff
// logging and an optional abort callback) until a matching id==3 response or id==4
// terminator shows up in receQueue, returning it via `respond`. Returns 1 on a response
// (id==3), 0 if disconnected, a terminator (id==4) was seen, or `cb` aborted the wait.
// CAVEAT: the decompile splits the scan position across two registers (v13/v20) that are
// always numerically equal (an erase decrements both, then both are unconditionally
// incremented, netting to "stay put"); reconstructed here as one `scanIndex`. `v29`
// (0x4000000, assigned but never read) is a decompiler dead store, dropped here.
int MSG_PROCESSOR_BASE::SendImmMsg(const void *data, int datalen, CON_MSG &respond, int (**cb)())
{
    if (!isConnected)
        return 0;

    // Purge any stale id==2 (previously queued, unanswered) requests before sending ours.
    lock.Lock(nullptr, 0);
    int liveCount = receQueue.nElem;
    int purgedRequestCount = 0;
    for (int i = 0; i < liveCount; liveCount = receQueue.nElem) {
        if (receQueue[i].id == 2) {
            ++purgedRequestCount;
            receQueue.Erase(i, 1);
        } else {
            ++i;
        }
    }
    int scanIndex = liveCount; // queue length at purge time -- messages before this index predate our request
    lock.Unlock(nullptr, 0);

    for (int n = purgedRequestCount; n > 0; --n)
        SendDiscardNotification(this);

    unsigned int startTime = osGetTime();
    int backoffSec = 1;
    int responseSuccess = 1; // becomes 0 if a terminator (id==4) is seen instead of a response

    SendMsg(data, datalen, 2);
    if (!isConnected)
        return 0;

    for (;;) {
        osSleep(1);
        if (1000 * backoffSec + startTime < osGetTime()) {
            _apLog("MSG_PROCESSOR_BASE::SendImmMsg - waiting... (%d)\n", backoffSec);
            backoffSec *= 2;
        }

        int staleRequestsSeen = 0;
        bool gotResult = false;

        lock.Lock(nullptr, 0);
        if (scanIndex < receQueue.nElem) {
            for (;;) {
                if (!IGNORE_STRONG_ASSERT && (scanIndex < 0 || scanIndex >= receQueue.nElem)) {
                    STRONG_ASSERT2_HELPER::asserd<int, int>(
                        "idx >= 0 && idx < nElem",
                        "d:\\projects\\code\\common\\incl.sys\\ds\\ds_vector.hpp",
                        567,
                        "idx", scanIndex,
                        "nElem", receQueue.nElem);
                }

                if (receQueue.pData[scanIndex].id == 3) {
                    respond.id = 3;
                    CON_MSG *matched = &receQueue[scanIndex];
                    unsigned char *tmpData   = respond.data.pData;
                    respond.data.pData        = matched->data.pData;
                    matched->data.pData       = tmpData;
                    int tmpNElem             = respond.data.nElem;
                    respond.data.nElem        = matched->data.nElem;
                    matched->data.nElem       = tmpNElem;
                    int tmpAllocated         = respond.data.allocated;
                    respond.data.allocated    = matched->data.allocated;
                    matched->data.allocated   = tmpAllocated;
                    receQueue.Erase(scanIndex, 1);
                    gotResult = true;
                    break;
                }

                if (!IGNORE_STRONG_ASSERT && (scanIndex < 0 || scanIndex >= receQueue.nElem)) {
                    STRONG_ASSERT2_HELPER::asserd<int, int>(
                        "idx >= 0 && idx < nElem",
                        "d:\\projects\\code\\common\\incl.sys\\ds\\ds_vector.hpp",
                        567,
                        "idx", scanIndex,
                        "nElem", receQueue.nElem);
                }

                if (receQueue.pData[scanIndex].id == 4) {
                    // Terminator: erase it, report failure, and stop scanning.
                    receQueue.Erase(scanIndex, 1);
                    responseSuccess = 0;
                    gotResult = true;
                    break;
                }

                if (!IGNORE_STRONG_ASSERT && (scanIndex < 0 || scanIndex >= receQueue.nElem)) {
                    STRONG_ASSERT2_HELPER::asserd<int, int>(
                        "idx >= 0 && idx < nElem",
                        "d:\\projects\\code\\common\\incl.sys\\ds\\ds_vector.hpp",
                        567,
                        "idx", scanIndex,
                        "nElem", receQueue.nElem);
                }

                if (receQueue.pData[scanIndex].id == 2) {
                    ++staleRequestsSeen;
                    receQueue.Erase(scanIndex, 1);
                } else {
                    ++scanIndex;
                }

                if (scanIndex >= receQueue.nElem)
                    break; // exhausted the queue this poll without a result
            }
        }
        lock.Unlock(nullptr, 0);

        for (int n = staleRequestsSeen; n > 0; --n)
            SendDiscardNotification(this);

        if (gotResult)
            return responseSuccess;
        if (cb && !(*cb)())
            return 0;
        if (!isConnected)
            return 0;
    }
}

} // namespace con
