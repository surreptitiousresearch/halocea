// haloENGINE_CONTROL__SetCurCheckpoint @0x823C4B50
#include "../../headers/hcex/haloENGINE_CONTROL.h"

// haloENGINE_CONTROL::SetCurCheckpoint @ 0x823C4B54 — set the current checkpoint name.
void haloENGINE_CONTROL::SetCurCheckpoint(const dsTSTRING<char> &chpt)
{
    curCheckpoint = chpt;
}
