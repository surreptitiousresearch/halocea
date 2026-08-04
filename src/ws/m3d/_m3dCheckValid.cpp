#include "m3d_boundary.h"

// _m3dCheckValid(float) @ 0x82659D18  (lower-address overload -> bare filename)
// Debug validity assert: true unless the value's IEEE-754 exponent field is all-ones
// (i.e. the value is +-Inf or NaN).
int _m3dCheckValid(float value)
{
    return (*(unsigned int *)&value & 0x7F800000) != 0x7F800000;
}
