#include "../../headers/hcex/HALO_SOUND_SYSTEM.h"

// HALO_SOUND_SYSTEM::PreloadSound(sound_permutation const*, bool) @ 0x836B5804
// Stub in this build: unconditionally reports success without touching any state. Both
// parameters are unused (confirmed against disasm -- the function body is a single `li r3,1;
// blr`, no reads of r3/r4/r5 besides the implicit `this`).
bool HALO_SOUND_SYSTEM::PreloadSound(const sound_permutation *perm, bool load)
{
    return true;
}
