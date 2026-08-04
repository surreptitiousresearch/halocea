#pragma once
/* d3dHCEX_DRIVER — HCEX's thin override of the ws-engine d3dDRIVER wrapper. It exists purely to
 * hook the legacy Blam rasterizer's own hardware init/reset/term into the ws render-driver
 * lifecycle (InitHW/ResetHW/TermHW). d3dDRIVER itself (and its vidDRIVER grandparent) is a large
 * pre-existing ws-engine class that is NOT part of this batch -- only the one field
 * (stereoCompatible) and the three lifecycle methods this override calls through to are modeled,
 * at their DB-verified offsets (types_members d3dDRIVER / d3dHCEX_DRIVER). d3dHCEX_DRIVER itself
 * adds no data members (DB size 2700 == base d3dDRIVER size exactly).
 *
 * DB-verified layout (types_members d3dHCEX_DRIVER): d3dDRIVER@0 (2700B base, size unchanged). */

struct d3dHCEX_DRIVER_vtbl; // boundary -- inherits vidDRIVER/d3dDRIVER's (huge) vtable shape

/* d3dDRIVER (base) -- full DB layout lives in src/headers/d3d_driver.h. */
#include "../d3d_driver.h"

/* Base-class lifecycle calls (super calls from the override) -- boundary. */
extern void d3dDRIVER_ctor(d3dDRIVER *self);
extern void d3dDRIVER_dtor(d3dDRIVER *self);
extern int  d3dDRIVER_InitHW(d3dDRIVER *self);
extern int  d3dDRIVER_ResetHW(d3dDRIVER *self);
extern void d3dDRIVER_TermHW(d3dDRIVER *self);

/* Legacy Blam rasterizer hooks (Blam engine boundary -- see CLAUDE.md Blam Subsystem Block).
 * Plain (unmangled) C symbols in the database -- extern "C" linkage. */
extern "C" {
    int rasterizer_inithw(int priorInitResult);   // real prototype takes one int (DB-verified)
    int rasterizer_termhw(void);

    // HCEX render-target resize helper -- reversed elsewhere (src/hcex/hcex_resize_render_targets.c).
    int hcex_resize_render_targets(int priorInitHwResult);
}

/* Video driver critical section, guarding the stereo-compatible reset path. */
#include "../ws/vid/vidLOCK.h"
extern vidLOCK vidLock;

struct d3dHCEX_DRIVER : d3dDRIVER
{
    static d3dHCEX_DRIVER_vtbl vftable; // `d3dHCEX_DRIVER::`vftable'' -- boundary

    // 0x823CEA04 -- construct the base d3dDRIVER, then re-point the vtable at this class's own.
    d3dHCEX_DRIVER();

    // 0x823CEB68 -- thunk to the base d3dDRIVER destructor.
    ~d3dHCEX_DRIVER();

    // 0x823D11F4 -- `scalar deleting destructor': run ~d3dHCEX_DRIVER(), then conditionally
    // operator-delete `this` when bit 0 of deleteFlags is set.
    d3dHCEX_DRIVER *ScalarDeletingDtor(unsigned char deleteFlags);

    // 0x823CEA9C -- run the base HW init, then bring the legacy Blam rasterizer's own hardware
    // state up alongside it.
    int InitHW();

    // 0x823CEB04 -- when this adapter supports stereo 3D, take vidLock and cycle the legacy Blam
    // rasterizer (term, base ResetHW, re-init, resize its render targets) around it. Always
    // returns 1 (matching the disassembly -- the base ResetHW result is consumed only to feed
    // the rasterizer re-init, never returned).
    int ResetHW();

    // 0x823CEAD4 -- term the legacy Blam rasterizer before tearing down the base HW state.
    void TermHW();
};
