#include "../../headers/ws/ai/aiEXIFACE.h"
#include "../../headers/ws/ai/aiEXIFACE_boundaries.h"

// aiEXIFACE::IsSkipDestr @ 0x82D31D18
// ?IsSkipDestr@aiEXIFACE@@QAA_NPAVentENTITY@@@Z
//
// True when `shooter` is an aiBRAIN (RTTI test) AND its virtual "skip destruction" predicate
// (entENTITY vtable slot 0x1B8) returns true.
bool aiEXIFACE::IsSkipDestr(entENTITY *shooter)
{
    bool isBrain = shooter && gsIs<aiBRAIN>(reinterpret_cast<const msgADDR *>(shooter)) != 0;
    if (!isBrain || !shooter)
        return false;

    typedef bool (*SkipDestrFn)(entENTITY *);
    void **vtbl = *reinterpret_cast<void ***>(shooter);
    return reinterpret_cast<SkipDestrFn>(vtbl[0x1B8 / sizeof(void *)])(shooter);
}
