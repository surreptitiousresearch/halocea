#include "../../headers/ws/ai/aiBRAIN.h"

// aiBRAIN::SetupSubSystems @ 0x83176270
// ?SetupSubSystems@aiBRAIN@@MAAXXZ  (protected virtual)
//
// Build the brain's two subsystem halves: the body (movement/animation interface) and the mind
// (behaviour modules).
void aiBRAIN::SetupSubSystems()
{
    SetupBody();
    SetupMind();
}
