#include "../../headers/ws/ai/aiAIM_BASE.h"
#include "../../headers/ws/ds/dsTSTRING.h"

// aiAIM_BASE::GetGivenAim @ 0x83179290
// ?GetGivenAim@aiAIM_BASE@@UBA?AV?$dsTSTRING@D@@XZ
//
// Base-class default: return an empty string. The body constructs the result via
// UnsafeInit("", -1, 0) — i.e. from the empty C string.
dsTSTRING<char> aiAIM_BASE::GetGivenAim() const
{
    return dsTSTRING<char>("");
}
