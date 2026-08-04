#include "../../headers/hcex/HALO_SOUND_SYSTEM.h"

// HALO_SOUND_SYSTEM::`scalar deleting destructor'(uint) @ 0x836C1EA4
// Compiler-generated: runs the real destructor, then conditionally operator-deletes `this`
// when bit 0 of `deleteFlags` is set (the standard MSVC `??_G` pattern).
HALO_SOUND_SYSTEM *HALO_SOUND_SYSTEM::scalarDeletingDtor(unsigned char deleteFlags)
{
    this->~HALO_SOUND_SYSTEM();
    if (deleteFlags & 1)
        operator delete(this);
    return this;
}
