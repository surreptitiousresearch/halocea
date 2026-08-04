#include "../../headers/ws/ai/aiWAH_SYS.h"
#include "../../headers/ws/ai/aiWTC_HOLDER.h"
#include "../../headers/ws/ai/aiWATCHER_ENT.h"
#include "../../headers/ws/ai/aiWAH_SYS_boundaries.h"
#include "../../headers/ws/ds/dsVECTOR.h"

// aiWAH_SYS::DropAll() @ 0x831722D8
// ?DropAll@aiWAH_SYS@@QAAXXZ
//
// Tear down every tracked player, entity and unit watcher, then clamp each surviving holder's
// counter to at most 1. (The per-element STRONG_ASSERT bounds checks the decompiler showed are
// the inlined dsVECTOR::operator[] guard — kept implicit here via operator[].)
void aiWAH_SYS::DropAll()
{
    arrPlayers.nElem = 0;

    const int playerCount = gsGetPlayerCount();
    for (int j = 0; j < playerCount; ++j)
        Forgo(gsGetPlayer(j));

    // arrEnts elements are aiWATCHER_ENT*, dropped through the aiWATCHER* overload (its base at
    // offset 0).
    const int nEnts = arrEnts.nElem;
    for (int i = 0; i < nEnts; ++i)
        Forgo(reinterpret_cast<aiWATCHER *>(arrEnts.pData[i]));
    arrEnts.nElem = 0;

    // Null out unit-watcher slots (they are owned/torn down elsewhere) and clear the list.
    const int nUnits = arrUnits.nElem;
    for (int i = 0; i < nUnits; ++i)
        arrUnits.pData[i] = nullptr;
    arrUnits.nElem = 0;

    // Reset any lingering demand counters to a single reference.
    const int nHolders = storage.nElem;
    for (int i = 0; i < nHolders; ++i)
    {
        aiWTC_HOLDER *holder = storage.pData[i];
        if (holder->counter > 0)
            holder->counter = 1;
    }
}
