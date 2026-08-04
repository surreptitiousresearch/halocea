#pragma once
// ws-engine ai08/nav: nav-system wrapper bases. DB-verified layouts:
//   aiNAV_NS_WRAPPER (types_members) — size 4: vtable pointer only.
//   aiNAV_NS_WRAPPER_GS — size 8: aiNAV_NS_WRAPPER base@0, navManager@4.
// Explicit __vftable member + no virtual methods (cdtREFINE precedent); vtable/methods are the
// next frontier.  boundary.

struct aiNAV_NS_WRAPPER_vtbl; // vtable  boundary (fwd)
struct navMANAGER;            // nav subsystem  boundary (fwd)

struct aiNAV_NS_WRAPPER {
    aiNAV_NS_WRAPPER_vtbl *__vftable; // 0x00
};

struct aiNAV_NS_WRAPPER_GS : aiNAV_NS_WRAPPER {
    navMANAGER *navManager; // 0x04 owning nav manager
};
