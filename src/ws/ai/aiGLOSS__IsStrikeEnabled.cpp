#include "../../headers/ws/ai/aiGLOSS.h"
#include "../../headers/ws/ai/aiGLOSS_local.h"
#include "../../headers/ws/wpn/wpnS_DESC.h"

// 0x8323A760 — aiGLOSS::IsStrikeEnabled. True when the descriptor's strike-sound name is NOT in
// the disabled list. Builds a temporary dsTSTRING from the interned strike name (the manual
// UnsafeInit / refCount-- / dlFree in the disassembly are the dsTSTRING ctor and dtor inlined).
bool aiGLOSS::IsStrikeEnabled(const wpnS_DESC &desc) const
{
    dsTSTRING<char> name(desc.nameStrike.CStr());
    bool enabled = ( FindInVector(&strikeDisabled, &name) == -1 );
    return enabled;
}
