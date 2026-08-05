// entENTITY::GetName @ 0x82534618 — return this entity's display name as a dsTSTRING<char>.
// If the entity has an animated instance (pInst), the name is taken from that instance
// (pInst->name, or the empty string when unset) into a fresh private buffer. Otherwise the
// entity's own actor name (iaIACTOR::nameIActive) is returned as a refcount-shared copy.
//
// The compiled function returns the dsTSTRING by value through a hidden first-argument buffer
// (r3); the entity `this` is r4. Reproduced here in natural by-value form (verified against the
// disassembly at 0x82534618: [this+0x15C]=pInst, [this+0xB4]=nameIActive).
#include "../../headers/ws/ent/entENTITY.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/ds/dsTSTRING_BUF_HEADER.h"

extern "C" const char empty_string[]; /* .rdata @0x8200155A - the shared "" literal (def: src/data/empty_string.c) */

dsTSTRING<char> entENTITY::GetName() const
{
    dsTSTRING<char> result;
    if (pInst)
    {
        const char *name = pInst->name ? pInst->name : empty_string;
        result.pBuffer = nullptr;
        result.UnsafeInit(name, -1, 0);
    }
    else
    {
        // share nameIActive's buffer: alias its header and bump the refcount
        result.pBuffer = nullptr;
        result.pBuffer = nameIActive.pBuffer;
        ++result.pBuffer->refCount;
    }
    return result;
}
