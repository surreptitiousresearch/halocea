#include <math.h>
#include "m3d_boundary.h"

// m3dCheckFinite(float) @ 0x8265BE20
// Non-throwing finiteness predicate: the value must be finite (per _m3dCheckValid) AND
// bounded by the engine's practical "safe float" range of +-4,000,000. On failure, raises
// the shipped STRONG_ASSERT (guarded by IGNORE_STRONG_ASSERT) via a null-`this` Crash call,
// then returns false either way.
bool m3dCheckFinite(float value)
{
    if (_m3dCheckValid(value))
        return fabsf(value) <= 4000000.0f;

    if (!IGNORE_STRONG_ASSERT)
        reinterpret_cast<STRONG_ASSERT_DUMMY *>(0)->Crash(
            "!\"Invalid float value\"",
            "D:\\Projects\\code\\common\\INCL.SYS\\m3d.h", 213, m3dStrongAssertMessage);

    return false;
}
