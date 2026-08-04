#include "mtlTBL.h"

// mtlTBL::Init @ 0x82C2ABD8
// Reset the texel-density table to its default (unscaled, 1.0) state for every slot.
// The decompiler modeled the writes as a raw float-pointer walk starting at
// &this->mtlList.__cl.line (an incidental cast of an unrelated apCL field) with the two
// interleaved stores `p_line[6] = 1.0` / `*++p_line = 1.0`; disassembly at 0x82C2AC48/
// 0x82C2AC84 confirms the two stores land at (this+16)+0x18+4*i = this+40+4*i and
// (this+16)+4+4*i = this+20+4*i for i in 0..4 — i.e. texDensity.v[i] and texDensity.u[i]
// (texDensity starts at this+20; v[] starts at this+40). Reconstructed as the two typed
// array-fill loops below; the STRONG_ASSERT bounds checks (ds_const_list.h:115, Size==5)
// are the inlined ds::ARRAY<float,5>::operator[] accessor and are not user-visible logic.
void mtlTBL::Init()
{
    for (int density_index = 0; density_index < 5; ++density_index)
    {
        this->texDensity.v.list[density_index] = 1.0f;
        this->texDensity.u.list[density_index] = 1.0f;
    }
}
