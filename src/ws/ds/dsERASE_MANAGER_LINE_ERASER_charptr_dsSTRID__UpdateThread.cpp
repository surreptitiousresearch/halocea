#include "dsERASE_MANAGER.h"
#include "dsLF_SORTED_MAP.h"
#include "dsSTRID.h"
#include "ds_assert_boundary.h"

extern "C" void dlFree(void *ptr); // boundary — dlmalloc free

struct dsSTR_HASH; // boundary — const char* hashing policy
struct dsSTR_CMP;  // boundary — const char* ordering policy

typedef dsLF_SORTED_MAP<const char *, dsSTRID, dsSTR_HASH, dsSTR_CMP>::LINE_ERASER StrStridLineEraser;

// Per-thread lock-free-container registration record, reached through the r13 TLS slot. For this
// manager type the assigned allocation ordinal lives at offset 12. // boundary
struct dsLF_THREAD_INFO {
    char _pad[12]; // 0x00
    int  slotIndex; // 0x0C  assigned allocation ordinal, or -1 when this thread is unregistered
};
dsLF_THREAD_INFO *dsLF_GetThreadInfo(); // boundary — reads the record pointer from TLS (r13)

// dsERASE_MANAGER<dsLF_SORTED_MAP<char const*,dsSTRID,dsSTR_HASH,dsSTR_CMP>::LINE_ERASER>
//   ::UpdateThread @ 0x8252B1A8
// Reclaim queued erases on behalf of the calling thread. No-op unless the thread owns an allocation
// slot. Under the access lock: pop-and-free every front entry whose threadMask already covers all
// OTHER registered threads' bits (i.e. every other thread has acknowledged the erase); stop at the
// first entry not yet fully acknowledged. Then stamp this thread's bit into every remaining entry.
template<>
void dsERASE_MANAGER<StrStridLineEraser>::UpdateThread()
{
    dsLF_THREAD_INFO *threadInfo = dsLF_GetThreadInfo();
    int threadSlot = threadInfo->slotIndex;
    if (threadSlot == -1)
        return; // this thread never registered with the manager

    osLOCK *p_accessLock = &this->accessLock;
    p_accessLock->Lock("D:\\Projects\\code\\common\\incl.sys\\ds\\ds_lf_map.h", 555);

    // Bits of all OTHER registered threads (this thread's own bit cleared).
    unsigned int otherThreads = this->threadAllocMask & ~(1u << threadSlot);

    while (this->eraseQueue.length) {
        if (!IGNORE_STRONG_ASSERT && this->eraseQueue.length <= 0)
            STRONG_ASSERT_DUMMY().Crash(
                "Length() > 0",
                "d:\\projects\\code\\common\\incl.sys\\ds\\ds_deque.hpp",
                298,
                empty_string);

        ERASED *front = &this->eraseQueue.data[this->eraseQueue.head];
        if (otherThreads != (front->threadMask & otherThreads))
            break; // not every other thread has acknowledged this erase yet

        dlFree(front->eraser.line);

        // Pop the front entry (ring-buffer bookkeeping, inlined dsDEQUE pop-front).
        int newLength = this->eraseQueue.length - 1;
        int capacity = this->eraseQueue.data.nElem;
        if (newLength > capacity) {
            this->eraseQueue.data.Resize(3 * newLength / 2);
            this->eraseQueue.Reconstruct(capacity);
        }
        if (newLength) {
            int newHead = this->eraseQueue.tail - newLength + 1;
            this->eraseQueue.head = newHead;
            if (newHead < 0)
                this->eraseQueue.head = this->eraseQueue.data.nElem + newHead;
        } else {
            this->eraseQueue.tail = 0;
            this->eraseQueue.head = 0;
        }
        this->eraseQueue.length = newLength;
    }

    // Mark every still-queued entry as seen by this thread.
    int remaining = this->eraseQueue.length;
    for (int j = 0; j < remaining; ++j) {
        int idx = (this->eraseQueue.head + j) % this->eraseQueue.data.nElem;
        this->eraseQueue.data[idx].threadMask |= 1u << threadSlot;
    }

    p_accessLock->Unlock("D:\\Projects\\code\\common\\incl.sys\\ds\\ds_lf_map.h", 555);
}
