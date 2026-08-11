#include "dsERASE_MANAGER.h"
#include "dsLF_SORTED_MAP.h"
#include "dsSTRID.h"
#include "ds_assert_boundary.h"

struct dsSTR_HASH; // boundary — const char* hashing policy
struct dsSTR_CMP;  // boundary — const char* ordering policy

typedef dsLF_SORTED_MAP<const char *, dsSTRID, dsSTR_HASH, dsSTR_CMP>::LINE_ERASER StrStridLineEraser;

// Per-thread lock-free-container registration record, reached through the r13 TLS slot. Only its
// slot-index field (offset 12) is read/written here; the rest is opaque. // boundary
struct dsLF_THREAD_INFO {
    char _pad[12]; // 0x00
    int  slotIndex; // 0x0C  assigned allocation ordinal, or negative when unregistered
};
dsLF_THREAD_INFO *dsLF_GetThreadInfo(); // boundary — reads the record pointer from TLS (r13)

// dsERASE_MANAGER<dsLF_SORTED_MAP<char const*,dsSTRID,dsSTR_HASH,dsSTR_CMP>::LINE_ERASER>
//   ::RegisterThread @ 0x82529478
// The first time the calling thread touches this manager, give it an allocation slot: under the
// access lock, scan threadAllocMask for the lowest free bit, claim it, and stamp its index into
// the thread's TLS record so subsequent traversals recognise the thread.
template<>
void dsERASE_MANAGER<StrStridLineEraser>::RegisterThread()
{
    dsLF_THREAD_INFO *threadInfo = dsLF_GetThreadInfo();
    if (threadInfo->slotIndex >= 0)
        return; // already registered

    osLOCK *p_accessLock = &this->accessLock;
    p_accessLock->Lock("D:\\Projects\\code\\common\\incl.sys\\ds\\ds_lf_map.h", 489);

    unsigned int mask = this->threadAllocMask;
    int numAllocated = 0;
    int idx = -1;
    // Original unrolls four bits per iteration via ROR; equivalently, scan bit 31 down to bit 0,
    // counting the set bits and remembering the lowest clear bit.
    for (int bit = 31; bit >= 0; --bit) {
        if (mask & (1u << bit))
            ++numAllocated;
        else
            idx = bit;
    }

    if (!IGNORE_STRONG_ASSERT && (numAllocated >= 32 || idx < 0))
        static_cast<STRONG_ASSERT_DUMMY *>(nullptr)->Crash(
            "numAllocated < 32 && idx >= 0",
            "D:\\Projects\\code\\common\\incl.sys\\ds\\ds_lf_map.h",
            501,
            empty_string);

    if (numAllocated < 32 && idx >= 0) {
        this->threadAllocMask |= 1 << idx;
        threadInfo->slotIndex = idx;
    }

    p_accessLock->Unlock("D:\\Projects\\code\\common\\incl.sys\\ds\\ds_lf_map.h", 489);
}
