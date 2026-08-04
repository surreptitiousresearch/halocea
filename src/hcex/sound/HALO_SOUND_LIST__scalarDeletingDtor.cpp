#include "../../headers/hcex/HALO_SOUND_LIST.h"

// HALO_SOUND_LIST::`scalar deleting destructor' @ 0x836B6FD4
// Compiler-generated: run the real destructor, then conditionally operator-delete `this` when
// bit 0 of `deleteFlags` is set (the "delete this object too" half of the scalar-deleting-dtor
// contract).
HALO_SOUND_LIST *HALO_SOUND_LIST::scalarDeletingDtor(unsigned char deleteFlags)
{
    this->~HALO_SOUND_LIST();
    if (deleteFlags & 1)
        operator delete(this);
    return this;
}
