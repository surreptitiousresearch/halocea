#pragma once
#include "../ds/dsTSTRING.h"
// ws-engine ai08: base class for a brain's approach ("aprog") behaviour module (aiMIND::aprog
// pointee). DB-verified size 0x4 (single vtable pointer). Concrete base — the default-system
// singleton (aiMIND::GetDefSys_Aprog) instantiates it directly. The layout keeps an explicit
// __vftable pointer (no compiler `virtual`) so sizeof stays exactly 4; the member wrappers below
// dispatch manually through the DB-verified vtable (types_members aiAPPROACH_BASE_vtbl).  boundary.

// DB-verified (types_members aiAPPROACH_BASE_vtbl).
struct aiAPPROACH_BASE_vtbl {
    void (__fastcall *dtr)(struct aiAPPROACH_BASE *self);                                  // 0x00
    void (__fastcall *ProcessFrame)(struct aiAPPROACH_BASE *self);                         // 0x04
    bool (__fastcall *SetByName)(struct aiAPPROACH_BASE *self, const dsTSTRING<char> *nm); // 0x08
    void (__fastcall *Enable)(struct aiAPPROACH_BASE *self, bool on);                      // 0x0C
    void (__fastcall *Disable)(struct aiAPPROACH_BASE *self);                              // 0x10
};

struct aiAPPROACH_BASE {
    aiAPPROACH_BASE_vtbl *__vftable; // 0x00

    // vtbl 0x04 — per-frame approach update. Used by aiMIND::ProcessFrameBeforeBody.
    void ProcessFrame() { __vftable->ProcessFrame(this); }
    // vtbl 0x08 — (re)configure this module from a named "approach" section.
    bool SetByName(const dsTSTRING<char> &name) { return __vftable->SetByName(this, &name); }
    // vtbl 0x0C — enable/disable the module.
    void Enable(bool on) { __vftable->Enable(this, on); }
    // vtbl 0x10 — disable the module.
    void Disable() { __vftable->Disable(this); }
};
