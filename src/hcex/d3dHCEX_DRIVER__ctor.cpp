/* d3dHCEX_DRIVER::d3dHCEX_DRIVER @0x823CEA04 — construct the base d3dDRIVER, then re-point the
 * vtable pointer at this derived class's own vtable. d3dDRIVER's ctor is a ws-engine boundary
 * (extern d3dDRIVER_ctor); its vtable pointer lives at offset 0 (vidDRIVER base __vftable), so
 * it is written via the raw offset-0 cast the disassembly shows (matching CrateHaloD3dDrv). */

#include "../headers/hcex/d3dHCEX_DRIVER.h"

d3dHCEX_DRIVER::d3dHCEX_DRIVER()
{
    d3dDRIVER_ctor(this);
    *(void **)this = (void *)&d3dHCEX_DRIVER::vftable;
}
