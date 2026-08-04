#include "../../headers/ws/os/os_boundary.h"

// SetThreadName @ 0x825E62D8 -- called from osCreateThread() to give the newly created
// thread a debugger-visible name. Classic VC++/Xbox 360 idiom: pack a THREADNAME_INFO-shaped
// record and raise the well-known 0x406D1388 ("MS_VC_EXCEPTION") debugger exception, which a
// debugger's exception filter recognizes and uses to rename the thread without ever actually
// stopping execution (RaiseException returns normally when no debugger is attached / the
// exception is handled and dismissed).
//
// DEVIATION: the decompiler shows a raw 4-word stack buffer; reproduced here as the standard
// THREADNAME_INFO layout (dwType=0x1000, szName, dwThreadID, dwFlags=0) that this pattern is
// always built from, since that matches the observed field order/values exactly
// (v2[0]=4096, v2[1]=szThreadName, v2[2]=dwThreadID, v2[3]=0).
struct THREADNAME_INFO
{
    unsigned int dwType;     // must be 0x1000
    const char  *szName;     // pointer to name (in user addr space)
    unsigned int dwThreadID; // thread ID (-1 = caller thread)
    unsigned int dwFlags;    // reserved for future use, must be zero
};

void SetThreadName(unsigned int dwThreadID, const char *szThreadName)
{
    THREADNAME_INFO info;
    info.dwType = 0x1000;
    info.szName = szThreadName;
    info.dwThreadID = dwThreadID;
    info.dwFlags = 0;

    RaiseException(0x406D1388u, 0, 4u, (const unsigned int *)&info);
}
