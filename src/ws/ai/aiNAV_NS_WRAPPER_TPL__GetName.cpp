// aiNAV_NS_WRAPPER_TPL::GetName @ 0x8321AC38
#include "../../headers/ws/ai/aiNAV_NS_WRAPPER_TPL.h"
#include "../../headers/ws/ds/dsTSTRING.h"

// Returns dsTSTRING<char> by value (sret). This template specialisation ignores nsIdx and yields
// the empty string — UnsafeInitEmpty adopts the shared refcounted empty buffer (bumping refCount),
// exactly as the binary inlines it.
dsTSTRING<char> aiNAV_NS_WRAPPER_TPL::GetName(int /*nsIdx*/) const
{
    dsTSTRING<char> result;
    result.UnsafeInitEmpty();
    return result;
}
