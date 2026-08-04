#include "../../headers/hcex/haloENGINE_CONTROL.h"

// haloENGINE_CONTROL::GetPathToCheckpoints @ 0x823C134C — root directory for checkpoint files.
const dsTSTRING<char> &haloENGINE_CONTROL::GetPathToCheckpoints() const
{
    return pathToCheckpoints;
}
