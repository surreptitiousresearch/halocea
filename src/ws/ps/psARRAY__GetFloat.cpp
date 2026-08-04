#include "psARRAY.h"
#include "../ds/dsDATA.h"
// 0x827AF500 — element `idx` coerced to float, falling back to `def` when the element is absent or
// not float-coercible. const (QBA).
//
// CAVEAT: the decompiler mis-shaped this via the PPC float-slot-skip ABI — it fabricated ~16 phantom
// double/float parameters (a4..a18) and returned `*((float*)&doubleTemp + 1)`. The mangle
// (?GetFloat@psARRAY@@QBAMHM@Z) is authoritative: `float GetFloat(int idx, float def) const`. The
// fabricated params collapse to the single seed value that `def` is copied into and psDataToFloat
// updates in place.

// ?psDataToFloat@@YAHABVdsDATA@@PAM@Z — coerce a dsDATA into a float (in/out). boundary.
extern int psDataToFloat(const dsDATA &data, float *value);

float psARRAY::GetFloat(int idx, float def) const
{
    float value = def;
    dsDATA element = this->GetElement(idx); // temp; destroyed at scope end
    psDataToFloat(element, &value);
    return value;
}
