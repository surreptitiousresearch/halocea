#include "../../headers/ws/ai/aiNAV_NS_WRAPPER_VERIFICATOR.h"
#include "../../headers/ws/ds/dsTSTRING.h"

// aiNAV_NS_WRAPPER_VERIFICATOR::GetName(int) const @ 0x8318FAE0
// Virtual base default: the verificator has no named nav systems, so every index maps to the
// fixed placeholder string "undefined". `nsIdx` is unused.
dsTSTRING<char> aiNAV_NS_WRAPPER_VERIFICATOR::GetName(int /*nsIdx*/) const
{
    dsTSTRING<char> result;
    result.pBuffer = nullptr;
    result.UnsafeInit("undefined", -1, 0);
    return result;
}
