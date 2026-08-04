// d3dHCEX_DRIVER -- HCEX's D3D driver wrapper: layers the legacy Blam rasterizer's own
// hardware lifecycle (rasterizer_inithw/rasterizer_termhw) on top of the ws-engine d3dDRIVER's.
// See src/headers/hcex/d3dHCEX_DRIVER.h for the DB-verified layout and boundary declarations.

#include "../../headers/hcex/d3dHCEX_DRIVER.h"

// 0x823CEA04
d3dHCEX_DRIVER::d3dHCEX_DRIVER()
{
    d3dDRIVER_ctor(this);
    this->base.__vftable = (vidDRIVER_vtbl *)&d3dHCEX_DRIVER::vftable; // vtable ptr lives in the vidDRIVER base member
}

// 0x823CEB68 -- thunk to the base destructor (no d3dHCEX_DRIVER-specific teardown).
d3dHCEX_DRIVER::~d3dHCEX_DRIVER()
{
    d3dDRIVER_dtor(this);
}

// 0x823D11F4
d3dHCEX_DRIVER *d3dHCEX_DRIVER::ScalarDeletingDtor(unsigned char deleteFlags)
{
    d3dDRIVER_dtor(this); // matches disasm: only the base dtor runs here, not the derived one
    if (deleteFlags & 1)
        operator delete(this);
    return this;
}

// 0x823CEA9C
int d3dHCEX_DRIVER::InitHW()
{
    int baseInited = d3dDRIVER_InitHW(this);
    // DEVIATION: disasm shows `bl rasterizer_inithw` with no argument-setup instruction between
    // it and the preceding call -- r3 still holds d3dDRIVER::InitHW's return value, so the real
    // call is rasterizer_inithw(baseInited), not rasterizer_inithw() as the decompiler printed.
    rasterizer_inithw(baseInited);
    return baseInited;
}

// 0x823CEB04
int d3dHCEX_DRIVER::ResetHW()
{
    if (this->stereoCompatible)
    {
        vidLock.Lock("D:\\Projects\\code\\HCEX\\sources\\halo_main.cpp", 2497);
        rasterizer_termhw();
        int baseReset = d3dDRIVER_ResetHW(this);
        // DEVIATION: same dead-arg-carry-through pattern as InitHW -- rasterizer_inithw and
        // hcex_resize_render_targets each take the previous call's return value implicitly (no
        // intervening r3 setup in the disasm), not zero-arg calls as the decompiler showed.
        int rasterInited = rasterizer_inithw(baseReset);
        hcex_resize_render_targets(rasterInited);
        vidLock.Unlock("D:\\Projects\\code\\HCEX\\sources\\halo_main.cpp", 2502);
    }
    return 1;
}

// 0x823CEAD4
void d3dHCEX_DRIVER::TermHW()
{
    rasterizer_termhw();
    d3dDRIVER_TermHW(this);
}
