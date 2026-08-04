#pragma once
#include "../m3d/m3dV.h"
// ws-engine ai08: base of the "isa" (interop/social-awareness) mind module. Only the one virtual
// aiPLANNER::AlertTeam dispatches is modelled here — the full layout is the next frontier.

struct entSND_NOTICE; // ws/ent — sound-notice event (passed by const ref only)  boundary

struct aiISA_BASE {
    void *__vftable; // 0x00 — module vtable (opaque boundary)

    // ?NotifySND@aiISA_BASE@@UAAXABVentSND_NOTICE@@@Z — virtual: notify this mind of sound event
    // `notice`. Used by aiPLANNER::ProcessSound.  boundary — body external to this batch.
    void NotifySND(const entSND_NOTICE &notice);

    // 0x831751A8 (?NotifyInterop@aiISA_BASE@@UAAXABUm3dV@@@Z) — virtual: notify this mind that an
    // interop/alert event happened at `pos`.  boundary — body external to this batch.
    virtual void NotifyInterop(const m3dV &pos);

    // vtbl+0x30 — apply a named-tunable value (the isa module takes only the float value; the key
    // has already been matched by the caller). Returns true if consumed. Used by aiMIND::SetPar.
    // boundary — body external to this batch.
    bool SetPar(float val);

    // ?IsZero@aiISA_BASE@@UBA_NXZ — virtual const: true when this is the null/"zero" isa arm (no
    // social/interop state). Used by aiMIND::IsIdle.  boundary.
    bool IsZero() const;
    // ?IsAlerted@aiISA_BASE@@UBA_NXZ — virtual const: true when the unit is currently alerted.
    // Used by aiMIND::IsIdle.  boundary.
    bool IsAlerted() const;
    // ?NotifyPhysObject@aiISA_BASE@@UAAXABUm3dV@@@Z — virtual: notify this mind that a physics
    // object passed near `pos`. Used by aiMIND::NoticeFlyObject.  boundary.
    void NotifyPhysObject(const m3dV &pos);

    // ?ProcessFRAME@aiISA_BASE@@... — virtual: per-frame isa (interop/social) update. Used by
    // aiMIND::UpdateAllPerception.  boundary.
    void ProcessFRAME();
};
