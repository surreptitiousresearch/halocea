// HCEX_DECAL — one entry in the hcexCreateDecals vector: a decal to be spawned at a bridged world
// position, plus its tag name. POD-ish boilerplate: default/copy ctor, dtor, scalar deleting dtor.
// See src/headers/hcex/HCEX_DECAL.h for the DB-verified layout.

#include "../../headers/hcex/HCEX_DECAL.h"
#include "../../headers/hcex/hcex_ds_boundary.h" // dlFree (C export)

// DEVIATION: HCEX_DECAL.h models `name` as the flat (non-template) hcex boundary `dsTSTRING` --
// a trivial struct with no declared destructor (unlike the C++ template dsTSTRING<char> used by
// the other classes in this cluster). There is therefore no implicit member destructor to lean on
// here: the refcount release below is genuine, explicit body code (matching the sibling bridge
// files, e.g. hcex_destroy_light.c, that manage this same flat dsTSTRING/ds_WEAK_PTR style by hand).

// @0x823D6918 — default-construct: name adopts the shared empty-string singleton via
// UnsafeInitEmpty(); pos is left uninitialized, matching the disassembly.
HCEX_DECAL::HCEX_DECAL()
{
    name.pBuffer = nullptr;
    name.UnsafeInitEmpty(); // canonical dsTSTRING<char> method (was flat dsTSTRING_UnsafeInitEmpty)
}

// 0x823D3EF0 — copy-construct: copy pos by value, then share that.name's buffer (ref-bump).
HCEX_DECAL::HCEX_DECAL(const HCEX_DECAL &that)
{
    pos = that.pos;
    name.pBuffer = nullptr;
    dsTSTRING_BUF_HEADER<char> *buffer = that.name.pBuffer;
    name.pBuffer = buffer;
    ++buffer->refCount;
}

// 0x823D3158 — release name's buffer reference, freeing it at zero.
HCEX_DECAL::~HCEX_DECAL()
{
    dsTSTRING_BUF_HEADER<char> *buffer = name.pBuffer;
    if (buffer->refCount-- == 1)
        dlFree(buffer);
}

// 0x823D3E98 (`??_G` mangle) — scalar deleting destructor: run ~HCEX_DECAL(), then conditionally
// operator-delete `this` when bit 0 of deleteFlags is set.
HCEX_DECAL *HCEX_DECAL::ScalarDeletingDtor(unsigned char deleteFlags)
{
    this->~HCEX_DECAL();
    if (deleteFlags & 1)
        operator delete(this);
    return this;
}
