#include "../../headers/ws/ai/aiWAH_SYS.h"

// aiWAH_SYS::PeekActive @ 0x83171910
// ?PeekActive@aiWAH_SYS@@QBAPAVaiWATCHER_BRAIN@@H@Z
// The idx-th active unit watcher, or null when idx is out of range.
aiWATCHER_BRAIN *aiWAH_SYS::PeekActive(int idx) const
{
    if (idx < 0 || idx >= arrUnits.nElem)
        return nullptr;
    return arrUnits[idx]; // dsVECTOR::operator[] const -> aiWATCHER_BRAIN* const&
}
