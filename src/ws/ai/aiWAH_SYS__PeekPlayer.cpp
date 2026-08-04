#include "../../headers/ws/ai/aiWAH_SYS.h"

// aiWAH_SYS::PeekPlayer @ 0x831718C0
// ?PeekPlayer@aiWAH_SYS@@QBAPAVaiWATCHER@@H@Z
// The idx-th active player watcher, or null when idx is out of range.
aiWATCHER *aiWAH_SYS::PeekPlayer(int idx) const
{
    if (idx < 0 || idx >= arrPlayers.nElem)
        return nullptr;
    return arrPlayers[idx]; // dsVECTOR::operator[] const -> aiWATCHER* const&
}
