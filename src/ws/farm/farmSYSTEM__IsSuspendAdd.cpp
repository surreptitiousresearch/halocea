#include "../../headers/ws/farm/farmSYSTEM.h"

// farmSYSTEM::IsSuspendAdd @ 0x823B08F0 -- const getter for the "suspend farm additions" flag
// (byte @ 0x10C).
bool farmSYSTEM::IsSuspendAdd() const
{
    return isSuspendAdd;
}
