#include "../../headers/ws/gs/gsINP_SYSTEM.h"

// gsINP_SYSTEM::GetInput — const accessor for the physical input device subsystem.
inpINPUT *gsINP_SYSTEM::GetInput() const
{
    return input;
}
