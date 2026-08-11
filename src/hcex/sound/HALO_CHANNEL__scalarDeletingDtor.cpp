#include "../../headers/hcex/HALO_CHANNEL.h"

// HALO_CHANNEL::`scalar deleting destructor' @ 0x836B6158
// Compiler-generated: run the real destructor, then conditionally operator-delete `this` when
// bit 0 of `deleteFlags` is set.
HALO_CHANNEL *HALO_CHANNEL::scalarDeletingDtor(unsigned char deleteFlags)
{
    this->~HALO_CHANNEL();
    if (deleteFlags & 1)
        operator delete(this);
    return this;
}
