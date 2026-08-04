#include "m3dRND_GEN.h"

// m3dRND_GEN::RndInt @ 0x82561FA0
// Standard MSVC-style linear-congruential step; returns the high 15 bits (0..32767).
int m3dRND_GEN::RndInt()
{
    this->seed = 214013 * this->seed + 2531011;
    return (this->seed >> 16) & 0x7FFF;
}
