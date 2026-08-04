// ??_GHALO_PERM_SOUND@@QAAPAXI@Z -- 0x836B50A0
// The destructor's body (prevFadeOut/fadeIn STRONG_ASSERT) is inlined at this address in the
// binary; reproduced here as a call to ~HALO_PERM_SOUND() -- identical semantics, compilable.
#include "../../headers/hcex/HALO_PERM_SOUND.h"

HALO_PERM_SOUND *HALO_PERM_SOUND::scalarDeletingDtor(unsigned char deleteFlags)
{
    this->~HALO_PERM_SOUND();

    HALO_PERM_SOUND *result = this;
    if ((deleteFlags & 1) != 0)
    {
        operator delete(this);
        return this;
    }
    return result;
}
