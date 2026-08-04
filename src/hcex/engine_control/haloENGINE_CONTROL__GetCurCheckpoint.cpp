#include "../../headers/hcex/haloENGINE_CONTROL.h"

// haloENGINE_CONTROL::GetCurCheckpoint @ 0x823C1344 — name of the most recently loaded/saved checkpoint.
const dsTSTRING<char> &haloENGINE_CONTROL::GetCurCheckpoint() const
{
    return curCheckpoint;
}
