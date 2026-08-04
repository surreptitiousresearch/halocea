#include "../../headers/ws/ai/aiEXIFACE.h"
#include "../../headers/ws/ai/aiBRAIN_IFACE.h"
#include "../../headers/ws/ds/dsTSTRING.h"

// aiEXIFACE::GetDbgWatcheeName() @ 0x82D31DA0
// Return the display name of the brain the debugger is currently watching; when nothing is
// watched, return the literal "none". sret — the return dsTSTRING is built directly in the
// caller-supplied result buffer (r3). The decompiler rendered that buffer as the implicit
// `this`; the real aiEXIFACE `this` is unused here (GetBrainDebugWatchee is a global accessor).
dsTSTRING<char> aiEXIFACE::GetDbgWatcheeName()
{
    aiBRAIN_IFACE *watchee = GetBrainDebugWatchee();

    dsTSTRING<char> result;
    if (watchee)
    {
        // Virtual (sret) — the watched brain fills the result buffer with its own name.
        watchee->GetName(&result);
    }
    else
    {
        // Build the constant "none" in place, matching the disassembly's raw init (clear the
        // buffer pointer, then UnsafeInit from the literal with len=-1, reserve=0).
        result.pBuffer = nullptr;
        result.UnsafeInit("none", -1, 0);
    }
    return result;
}
