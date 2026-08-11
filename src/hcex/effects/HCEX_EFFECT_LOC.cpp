// HCEX_EFFECT_LOC — a single placed effect location (position + direction + name). POD-ish
// boilerplate: default/copy ctor, dtor, scalar deleting dtor. See src/headers/hcex/HCEX_EFFECT_LOC.h
// for the DB-verified layout.

#include "../../headers/hcex/HCEX_EFFECT_LOC.h"

// @0x823D6298 — default-construct. name adopts the shared empty-string singleton via
// UnsafeInitEmpty(); pos/vec are left uninitialized, matching the disassembly (no stores to
// this->pos/this->vec appear before the name init).
HCEX_EFFECT_LOC::HCEX_EFFECT_LOC()
{
    name.pBuffer = nullptr;
    name.UnsafeInitEmpty();
}

// 0x823D4670 — copy-construct: copy pos/vec by value, then share that.name's buffer (ref-bump).
HCEX_EFFECT_LOC::HCEX_EFFECT_LOC(const HCEX_EFFECT_LOC &that)
{
    pos = that.pos;
    vec = that.vec;
    name.pBuffer = nullptr;
    dsTSTRING_BUF_HEADER<char> *buffer = that.name.pBuffer;
    name.pBuffer = buffer;
    ++buffer->refCount;
}

// 0x823D1F78 — destructor body is empty: `name`'s destruction is the implicit per-member teardown
// (dsTSTRING<char>::~dsTSTRING(), declared in dsTSTRING.h). DEVIATION: the disassembly shows this
// call's body fully inlined (a raw refCount-- / dlFree pair) rather than a visible CALL instruction
// -- the original compiler inlined the tiny callee at this address. Reproducing that logic by hand
// here as well would double-release the buffer once ~dsTSTRING<char>() itself is given a body
// elsewhere in the corpus, so the correct reconstruction is to let the implicit member destructor
// do this exactly once.
HCEX_EFFECT_LOC::~HCEX_EFFECT_LOC()
{
}

// 0x823D3E30 (`??_G` mangle) — scalar deleting destructor: run ~HCEX_EFFECT_LOC(), then
// conditionally operator-delete `this` when bit 0 of deleteFlags is set.
HCEX_EFFECT_LOC *HCEX_EFFECT_LOC::ScalarDeletingDtor(unsigned char deleteFlags)
{
    this->~HCEX_EFFECT_LOC();
    if (deleteFlags & 1)
        operator delete(this);
    return this;
}
