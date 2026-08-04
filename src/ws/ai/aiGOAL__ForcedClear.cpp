#include "../../headers/ws/ai/aiGOAL.h"
#include "../../headers/ws/nav/navSYS.h"

// aiGOAL::ForcedClear() @ 0x832AEFE0
// ?ForcedClear@aiGOAL@@UAAXXZ  (virtual)
//
// Release the goal's owned nav-system (dropping the owning brain's reference) and clear the pointer.
void aiGOAL::ForcedClear()
{
    if (navSys)
        navSys->Release(pBrain);
    navSys = nullptr;
}
