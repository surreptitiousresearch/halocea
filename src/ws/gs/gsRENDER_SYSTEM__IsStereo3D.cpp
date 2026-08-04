#include "../../headers/ws/gs/gsRENDER_SYSTEM.h"

// 0x823CDD08 — const getter for the stereo-3D enable flag.
bool gsRENDER_SYSTEM::IsStereo3D() const
{
    return isStereo3D;
}
