// dsSTRID_UpdateThread @ 0x8252B5A8 — per-frame maintenance tick for the string-id interning
// system: run the deferred-erase pass for the string→dsSTRID map's lock-free line eraser.
#include "dsERASE_MANAGER.h"
#include "dsLF_SORTED_MAP.h"
#include "dsSTRID.h"

struct dsSTR_HASH; // boundary — const char* hashing policy
struct dsSTR_CMP;  // boundary — const char* ordering policy

typedef dsLF_SORTED_MAP<const char *, dsSTRID, dsSTR_HASH, dsSTR_CMP> StrStridMap;

void dsSTRID_UpdateThread()
{
    dsERASE_MANAGER<StrStridMap::LINE_ERASER> *eraseMgr = StrStridMap::GetEraseMgr();
    eraseMgr->UpdateThread();
}
