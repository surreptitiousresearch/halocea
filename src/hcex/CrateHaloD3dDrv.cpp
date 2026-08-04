/* Reconstruction (no DB/PDB type) — adjudicated KEEP, see .complete/ESCALATIONS.md */
/* CrateHaloD3dDrv @0x823D1168 — the real DB symbol (note the baked-in typo: "Crate" not "Create").
 * Allocates a d3dDRIVER, runs its base constructor, then overwrites its vtable pointer to the
 * HCEX-specific derived vtable (d3dHCEX_DRIVER) without running d3dHCEX_DRIVER's own constructor
 * -- a "fake polymorphism" pattern used elsewhere in this bridge layer to attach a derived
 * vtable to an already-initialized base object. `d3dDRIVER` is an opaque ws-engine boundary type
 * here (see hcex_render_present_boundary.h); its vtable pointer is written via the raw offset-0
 * cast the disassembly shows, since no typed field is exposed on the boundary struct. */

#include "../headers/hcex/hcex_render_present_boundary.h" /* d3dDRIVER (opaque) */

extern void *operator new(size_t size, const char *file, int line);
extern void  d3dDRIVER_ctor(d3dDRIVER *self); /* d3dDRIVER::d3dDRIVER @0x8268F140 */
extern void *d3dHCEX_DRIVER_vftable;          /* d3dHCEX_DRIVER::`vftable' */

/* extern "C": the real DB symbol is the unmangled C-linkage name `CrateHaloD3dDrv` (see the typo),
 * assigned to the C function-pointer D3D_DRV_CREATOR; the caller declares it extern "C" too. */
extern "C" d3dDRIVER *CrateHaloD3dDrv(void)
{
    d3dDRIVER *self = (d3dDRIVER *)operator new(0xA8Cu, "D:\\Projects\\code\\HCEX\\sources\\halo_main.cpp", 0x9D1);

    if (!self)
        return 0;

    d3dDRIVER_ctor(self);
    *(void **)self = &d3dHCEX_DRIVER_vftable;
    return self;
}
