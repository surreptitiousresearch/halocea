// dsUpdateEventMgr @ 0x8261A9D0 — per-frame maintenance tick for the ws-engine event system:
// run the deferred-erase pass for the int-keyed string map's lock-free line eraser, then pump the
// global state manager's thread update.
#include "dsERASE_MANAGER.h"
#include "dsLF_SORTED_MAP.h"

struct dsSTR_HASH; // boundary — const char* hashing policy
struct dsSTR_CMP;  // boundary — const char* ordering policy

typedef dsLF_SORTED_MAP<const char *, int, dsSTR_HASH, dsSTR_CMP> StrIntMap;

// dsSTATE_MGR::UpdateThread(gStateMgr) — global state-manager per-thread pump. // boundary
struct dsSTATE_MGR;
extern dsSTATE_MGR *gStateMgr;
void dsSTATE_MGR_UpdateThread(dsSTATE_MGR *mgr); // boundary — ?UpdateThread@dsSTATE_MGR@@QAAXXZ

void dsUpdateEventMgr()
{
    dsERASE_MANAGER<StrIntMap::LINE_ERASER> *eraseMgr = StrIntMap::GetEraseMgr();
    eraseMgr->UpdateThread();
    dsSTATE_MGR_UpdateThread(gStateMgr);
}
