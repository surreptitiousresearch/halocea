#include "m3d_boundary.h"

// _m3dCheckValid(const m3dV*) @ 0x82659DD8  (higher-address overload -> __2)
// Debug validity assert over all three components: true unless any component's IEEE-754
// exponent field is all-ones (+-Inf or NaN).
int _m3dCheckValid(const m3dV *v)
{
    return (*(unsigned int *)&v->x & 0x7F800000) != 0x7F800000
        && (*(unsigned int *)&v->y & 0x7F800000) != 0x7F800000
        && (*(unsigned int *)&v->z & 0x7F800000) != 0x7F800000;
}
