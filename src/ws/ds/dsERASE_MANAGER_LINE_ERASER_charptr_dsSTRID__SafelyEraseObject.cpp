#include "dsERASE_MANAGER.h"
#include "dsLF_SORTED_MAP.h"
#include "dsSTRID.h"
#include "ds_assert_boundary.h"

struct dsSTR_HASH; // boundary — const char* hashing policy
struct dsSTR_CMP;  // boundary — const char* ordering policy

typedef dsLF_SORTED_MAP<const char *, dsSTRID, dsSTR_HASH, dsSTR_CMP> StrStridMap;

// dsERASE_MANAGER<dsLF_SORTED_MAP<const char*,dsSTRID,dsSTR_HASH,dsSTR_CMP>::LINE_ERASER>
//   ::SafelyEraseObject @ 0x8252B368
// Under the access lock, enqueue `eraser` for deferred destruction. The queued record keeps the
// erased LINE pointer as a dedup/ownership key alongside the full cursor. Every 100th enqueue
// triggers a reclamation pass (UpdateThread).
template<>
void dsERASE_MANAGER<StrStridMap::LINE_ERASER>::SafelyEraseObject(const StrStridMap::LINE_ERASER &eraser)
{
    osLOCK *p_accessLock = &this->accessLock;
    p_accessLock->Lock("D:\\Projects\\code\\common\\incl.sys\\ds\\ds_lf_map.h", 534);

    if (!IGNORE_STRONG_ASSERT && this->eraseQueue.length >= 0x2000)
        static_cast<STRONG_ASSERT_DUMMY *>(nullptr)->Crash(
            "eraseQueue.Length() < 8192",
            "D:\\Projects\\code\\common\\incl.sys\\ds\\ds_lf_map.h",
            536,
            empty_string);

    ERASED rec;
    rec.threadMask = (unsigned int)eraser.line; // dedup key = erased LINE pointer
    rec.eraser = eraser;
    this->eraseQueue.PushBack(rec);

    if (this->eraseQueue.length % 100 == 0)
        this->UpdateThread();

    p_accessLock->Unlock("D:\\Projects\\code\\common\\incl.sys\\ds\\ds_lf_map.h", 534);
}
