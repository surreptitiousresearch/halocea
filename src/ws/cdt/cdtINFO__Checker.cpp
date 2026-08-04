// cdtINFO::Checker @0x82C16088 — reads bit 1 of the state word, identical accessor to IsValid()
// (distinct source name; same underlying flag).
#include "../../headers/ws/cdt/cdtINFO.h"

unsigned int cdtINFO::Checker() const
{
    return (state.state >> 1) & 1;
}
