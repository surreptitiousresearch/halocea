// HCEX_EFFECT — a named effect and its set of placed HCEX_EFFECT_LOC locations. POD-ish
// boilerplate: default/copy ctor, dtor, scalar deleting dtor. See src/headers/hcex/HCEX_EFFECT.h
// for the DB-verified layout.

#include "../../headers/hcex/HCEX_EFFECT.h"

// 0x823D6AA4 — default-construct: locations adopts the ds_vector.h:24 call-site cookie (empty
// vector), hcexName adopts the shared empty-string singleton via UnsafeInitEmpty().
HCEX_EFFECT::HCEX_EFFECT()
    : locations(apCL{"D:\\Projects\\code\\common\\INCL.SYS\\ds\\ds_vector.h", 24})
{
    hcexName.pBuffer = nullptr;
    hcexName.UnsafeInitEmpty();
}

// 0x823E4350 — copy-construct: adopt that.locations's call-site cookie, deep-copy its elements via
// Insert, then share that.hcexName's buffer (ref-bump).
HCEX_EFFECT::HCEX_EFFECT(const HCEX_EFFECT &that)
    : locations(that.locations.__cl)
{
    locations.Insert(0, that.locations.pData, that.locations.nElem);
    hcexName.pBuffer = nullptr;
    dsTSTRING_BUF_HEADER<char> *buffer = that.hcexName.pBuffer;
    hcexName.pBuffer = buffer;
    ++buffer->refCount;
}

// 0x823D6B54 — destructor body is empty: both members destruct via the implicit per-member
// teardown, in reverse declaration order (hcexName, then locations) -- matching the disasm's
// hcexName-refcount-release-then-locations-dtor-call sequence exactly. DEVIATION: the disasm
// shows hcexName's release fully inlined (dsTSTRING<char>::~dsTSTRING() is tiny) while locations's
// ~dsVECTOR<HCEX_EFFECT_LOC,8>() remains a visible CALL (it is not trivial-sized); both are still
// just the automatic member destruction the compiler emits for a destructor with no other explicit
// work, so no manual body is written here to avoid double-releasing either member.
HCEX_EFFECT::~HCEX_EFFECT()
{
}

// 0x823D7C10 (`??_G` mangle) — scalar deleting destructor: run ~HCEX_EFFECT(), then conditionally
// operator-delete `this` when bit 0 of deleteFlags is set.
HCEX_EFFECT *HCEX_EFFECT::ScalarDeletingDtor(unsigned char deleteFlags)
{
    this->~HCEX_EFFECT();
    if (deleteFlags & 1)
        operator delete(this);
    return this;
}
