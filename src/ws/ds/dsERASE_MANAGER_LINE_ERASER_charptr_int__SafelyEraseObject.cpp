#include "dsERASE_MANAGER.h"
#include "dsLF_SORTED_MAP.h"
#include "ds_assert_boundary.h"

struct dsSTR_HASH; // boundary — const char* hashing policy
struct dsSTR_CMP;  // boundary — const char* ordering policy

typedef dsLF_SORTED_MAP<const char *, int, dsSTR_HASH, dsSTR_CMP> StrIntMap;

// dsERASE_MANAGER<dsLF_SORTED_MAP<const char*,int,dsSTR_HASH,dsSTR_CMP>::LINE_ERASER>
//   ::SafelyEraseObject @ 0x8261A2E8
// Under the access lock, enqueue `eraser` for deferred destruction. The queued record keeps the
// erased LINE pointer as a dedup/ownership key alongside the full cursor. Every 100th enqueue
// triggers a reclamation pass (UpdateThread).
template<>
void dsERASE_MANAGER<StrIntMap::LINE_ERASER>::SafelyEraseObject(const StrIntMap::LINE_ERASER &eraser)
{
    osLOCK *p_accessLock = &this->accessLock;
    p_accessLock->Lock("D:\\Projects\\code\\common\\INCL.SYS\\ds/ds_lf_map.h", 534);

    if (!IGNORE_STRONG_ASSERT && this->eraseQueue.length >= 0x2000)
        STRONG_ASSERT_DUMMY().Crash(
            "eraseQueue.Length() < 8192",
            "D:\\Projects\\code\\common\\INCL.SYS\\ds/ds_lf_map.h",
            536,
            dsStrongAssertMessage);

    ERASED rec;
    rec.threadMask = (unsigned int)eraser.line; // dedup key = erased LINE pointer
    rec.eraser = eraser;
    this->eraseQueue.PushBack(rec);

    if (this->eraseQueue.length % 100 == 0)
        this->UpdateThread();

    p_accessLock->Unlock("D:\\Projects\\code\\common\\INCL.SYS\\ds/ds_lf_map.h", 534);
}
