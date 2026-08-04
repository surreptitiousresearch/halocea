#include "m3dV.h"
#include "m3dMATR.h"

extern void _m3dCheckValid(const m3dV *v); // boundary — vector validity assert, not reversed

// m3dMATR::GetOrigin @ 0x823CD458
// Extract the translation (origin) from the matrix's 4th row: elements[12..14].
void m3dMATR::GetOrigin(m3dV *org) const
{
    org->x = this->elements[12];
    org->y = this->elements[13];
    org->z = this->elements[14];
    _m3dCheckValid(org);
}
