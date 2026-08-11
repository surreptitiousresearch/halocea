#include "../../headers/hcex/haloENGINE_CONTROL.h"

// haloENGINE_CONTROL::EVENT_DISP_haloENGINE_CONTROL::`scalar deleting destructor' @ 0x823C9928 —
// compiler-generated (`??_G` mangle): runs ~EVENT_DISP_haloENGINE_CONTROL(), then conditionally
// operator-deletes `this` when bit 0 of `deleteFlags` is set.
haloENGINE_CONTROL::EVENT_DISP_haloENGINE_CONTROL *
haloENGINE_CONTROL::EVENT_DISP_haloENGINE_CONTROL::scalarDeletingDtor(unsigned char deleteFlags)
{
    this->~EVENT_DISP_haloENGINE_CONTROL();
    if (deleteFlags & 1)
        operator delete(this);
    return this;
}
