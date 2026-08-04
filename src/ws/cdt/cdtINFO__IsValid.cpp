// cdtINFO::IsValid @0x82C16078 — returns bit 1 of the state word (the "vClosest set" flag
// recorded into state), i.e. whether the collision record holds a usable closest-point result.
#include "../../headers/ws/cdt/cdtINFO.h"

unsigned int cdtINFO::IsValid() const
{
    return (state.state >> 1) & 1;
}
