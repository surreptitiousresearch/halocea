#pragma once
// ws-engine ai08 — boundary accessors for the aiWATCHER accessor batch (0x8328E800..0x8328F268).
// These reach fields of descriptor/processor types whose full layouts are reversed in other
// subsystems (the next re-source frontier). They are declared here as boundary free functions so
// this batch stays self-contained without fabricating those layouts.

struct aiBRAIN;        // ai08 — per-unit AI brain            boundary (fwd)
struct usePROCESSOR;   // use subsystem — hideout/ITRC proc    boundary (fwd)
struct mdlITRC_BASE;   // mdl subsystem — interactive transform ref (aiWATCHER_BRAIN_boundaries.h)
struct m3dV;           // m3d/m3dV.h — 3-vector                 boundary (fwd)

// m3d free operator ??D@YA?AUm3dV@@MABU0@@Z @0x82... — scalar * vector (returns m3dV by value).
// boundary (m3d subsystem).
struct m3dV operator*(float scalar, const struct m3dV &v);

// aiBRAIN::spDesc (iaIACTOR+0x54) owns a dscDESC; the per-species vertical/horizontal VC-adjust-HO
// magnitude is a single float at dscDESC+0x328 (descriptor layout reversed elsewhere).
// disasm: lwz r30,0x54(brain); lfs f,0x328(r30).  boundary raw-field accessor.
float aiBRAIN_GetVCAdjustHOAmount(const aiBRAIN *observer);

// usePROCESSOR+0x50 — the currently-active hideout ITRC (mdlITRC_BASE*), or null.  boundary
// raw-field accessor (usePROCESSOR layout reversed elsewhere).
mdlITRC_BASE *usePROCESSOR_GetCurrentItrc(const usePROCESSOR *proc);

// usePROCESSOR+0xC8 — the processor's context property list (ctrlPROP_LIST) fed to
// aiWATCHER::TranslateSTRID_FLAGS. disasm: addi r4, useProc, 0xC8.  boundary raw-field accessor.
struct ctrlPROP_LIST;
const ctrlPROP_LIST &usePROCESSOR_GetContextPropList(const usePROCESSOR *proc);
