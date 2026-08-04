#include "m3d_boundary.h"

// m3dVRandIntRange(const m3dV&, int, int, unsigned long) @ 0x82562C20
// Maps the low 15 bits of the m3dVRand hash into the integer range [lo, hi).
int m3dVRandIntRange(const m3dV &v, int lo, int hi, unsigned int stride)
{
    return (m3dVRand(v, stride) & 0x7FFF) * (hi - lo) / 0x7FFF + lo;
}
