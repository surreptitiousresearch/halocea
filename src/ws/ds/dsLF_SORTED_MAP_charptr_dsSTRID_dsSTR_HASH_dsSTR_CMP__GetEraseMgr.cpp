#include "dsLF_SORTED_MAP.h"
#include "dsSTRID.h"

struct dsSTR_HASH; // boundary — const char* hashing policy
struct dsSTR_CMP;  // boundary — const char* ordering policy

// dsLF_SORTED_MAP<const char*,dsSTRID,dsSTR_HASH,dsSTR_CMP>::GetEraseMgr @ 0x82529F48
// Return the process-wide deferred-erase manager for this map type. The manager is a
// function-local static, constructed on first call (compiler-generated one-time guard)
// and torn down via atexit.
template<>
dsERASE_MANAGER<dsLF_SORTED_MAP<const char *, dsSTRID, dsSTR_HASH, dsSTR_CMP>::LINE_ERASER> *
dsLF_SORTED_MAP<const char *, dsSTRID, dsSTR_HASH, dsSTR_CMP>::GetEraseMgr()
{
    static dsERASE_MANAGER<LINE_ERASER> eraseMgr;
    return &eraseMgr;
}
